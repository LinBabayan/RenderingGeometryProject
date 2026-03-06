#include "BezierSurface.h"

#include <cmath>

BezierSurface::BezierSurface() {}

BezierSurface::BezierSurface(const std::vector<std::vector<glm::vec3>>& controlGrid)
    : controlPoints(controlGrid) {}

void BezierSurface::setControlGrid(const std::vector<std::vector<glm::vec3>>& grid) {
    controlPoints = grid;
}

const std::vector<std::vector<glm::vec3>>& BezierSurface::getControlGrid() const {
    return controlPoints;
}

unsigned int BezierSurface::binomialCoefficient(unsigned int n, unsigned int k) const {
    if (k > n) {
        return 0;
    }
    if (k == 0 || k == n) {
        return 1;
    }

    if (k > n - k) {
        k = n - k;
    }

    unsigned int result = 1;
    for (unsigned int i = 0; i < k; ++i) {
        result *= (n - i);
        result /= (i + 1);
    }

    return result;
}

float BezierSurface::bernstein(unsigned int i, unsigned int n, float t) const {
    return static_cast<float>(binomialCoefficient(n, i)) *
           std::pow(1.0f - t, static_cast<int>(n - i)) *
           std::pow(t, static_cast<int>(i));
}

float BezierSurface::bernsteinDerivative(unsigned int i, unsigned int n, float t) const {
    if (n == 0) {
        return 0.0f;
    }

    float left = 0.0f;
    float right = 0.0f;

    if (i > 0) {
        left = bernstein(i - 1, n - 1, t);
    }
    if (i < n) {
        right = bernstein(i, n - 1, t);
    }

    return static_cast<float>(n) * (left - right);
}

glm::vec3 BezierSurface::evaluate(float u, float v) const {
    if (controlPoints.empty() || controlPoints[0].empty()) {
        return glm::vec3(0.0f);
    }

    unsigned int m = static_cast<unsigned int>(controlPoints.size() - 1);
    unsigned int n = static_cast<unsigned int>(controlPoints[0].size() - 1);

    glm::vec3 point(0.0f);

    for (unsigned int i = 0; i <= m; ++i) {
        float bu = bernstein(i, m, u);
        for (unsigned int j = 0; j <= n; ++j) {
            float bv = bernstein(j, n, v);
            point += bu * bv * controlPoints[i][j];
        }
    }

    return point;
}

glm::vec3 BezierSurface::evaluateDu(float u, float v) const {
    if (controlPoints.empty() || controlPoints[0].empty()) {
        return glm::vec3(0.0f);
    }

    unsigned int m = static_cast<unsigned int>(controlPoints.size() - 1);
    unsigned int n = static_cast<unsigned int>(controlPoints[0].size() - 1);

    glm::vec3 tangent(0.0f);

    for (unsigned int i = 0; i <= m; ++i) {
        float du = bernsteinDerivative(i, m, u);
        for (unsigned int j = 0; j <= n; ++j) {
            float bv = bernstein(j, n, v);
            tangent += du * bv * controlPoints[i][j];
        }
    }

    return tangent;
}

glm::vec3 BezierSurface::evaluateDv(float u, float v) const {
    if (controlPoints.empty() || controlPoints[0].empty()) {
        return glm::vec3(0.0f);
    }

    unsigned int m = static_cast<unsigned int>(controlPoints.size() - 1);
    unsigned int n = static_cast<unsigned int>(controlPoints[0].size() - 1);

    glm::vec3 tangent(0.0f);

    for (unsigned int i = 0; i <= m; ++i) {
        float bu = bernstein(i, m, u);
        for (unsigned int j = 0; j <= n; ++j) {
            float dv = bernsteinDerivative(j, n, v);
            tangent += bu * dv * controlPoints[i][j];
        }
    }

    return tangent;
}

std::vector<SurfaceVertex> BezierSurface::sampleUniform(int uSegments, int vSegments) const {
    std::vector<SurfaceVertex> triangles;

    if (uSegments < 1 || vSegments < 1 || controlPoints.empty() || controlPoints[0].empty()) {
        return triangles;
    }

    std::vector<std::vector<SurfaceVertex>> grid(
        static_cast<size_t>(uSegments + 1),
        std::vector<SurfaceVertex>(static_cast<size_t>(vSegments + 1))
    );

    for (int iu = 0; iu <= uSegments; ++iu) {
        float u = static_cast<float>(iu) / static_cast<float>(uSegments);

        for (int iv = 0; iv <= vSegments; ++iv) {
            float v = static_cast<float>(iv) / static_cast<float>(vSegments);

            glm::vec3 p = evaluate(u, v);
            glm::vec3 du = evaluateDu(u, v);
            glm::vec3 dv = evaluateDv(u, v);

            glm::vec3 normal = glm::cross(du, dv);
            if (glm::length(normal) > 1e-6f) {
                normal = glm::normalize(normal);
            } else {
                normal = glm::vec3(0.0f, 1.0f, 0.0f);
            }

            grid[static_cast<size_t>(iu)][static_cast<size_t>(iv)] = {p, normal, glm::vec2(u, v)};
        }
    }

    triangles.reserve(static_cast<size_t>(uSegments * vSegments * 6));

    for (int iu = 0; iu < uSegments; ++iu) {
        for (int iv = 0; iv < vSegments; ++iv) {
            const SurfaceVertex& p00 = grid[static_cast<size_t>(iu)][static_cast<size_t>(iv)];
            const SurfaceVertex& p10 = grid[static_cast<size_t>(iu + 1)][static_cast<size_t>(iv)];
            const SurfaceVertex& p01 = grid[static_cast<size_t>(iu)][static_cast<size_t>(iv + 1)];
            const SurfaceVertex& p11 = grid[static_cast<size_t>(iu + 1)][static_cast<size_t>(iv + 1)];

            triangles.push_back(p00);
            triangles.push_back(p10);
            triangles.push_back(p11);

            triangles.push_back(p00);
            triangles.push_back(p11);
            triangles.push_back(p01);
        }
    }

    return triangles;
}

std::vector<float> BezierSurface::sampleUniformInterleaved(int uSegments, int vSegments) const {
    std::vector<SurfaceVertex> vertices = sampleUniform(uSegments, vSegments);
    std::vector<float> data;
    data.reserve(vertices.size() * 8);

    for (const auto& vertex : vertices) {
        data.push_back(vertex.position.x);
        data.push_back(vertex.position.y);
        data.push_back(vertex.position.z);

        data.push_back(vertex.normal.x);
        data.push_back(vertex.normal.y);
        data.push_back(vertex.normal.z);

        data.push_back(vertex.uv.x);
        data.push_back(vertex.uv.y);
    }

    return data;
}
