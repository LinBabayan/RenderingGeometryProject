#include "BezierCurve.h"

BezierCurve::BezierCurve() {}

BezierCurve::BezierCurve(const std::vector<glm::vec3>& controlPts)
    : controlPoints(controlPts) {}

void BezierCurve::setControlPoints(const std::vector<glm::vec3>& controlPts) {
    controlPoints = controlPts;
}

const std::vector<glm::vec3>& BezierCurve::getControlPoints() const {
    return controlPoints;
}

unsigned int BezierCurve::binomialCoefficient(unsigned int n, unsigned int k) const {
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

float BezierCurve::bernstein(unsigned int i, unsigned int n, float u) const {
    float oneMinusU = 1.0f - u;
    return static_cast<float>(binomialCoefficient(n, i)) *
           pow(oneMinusU, static_cast<int>(n - i)) *
           pow(u, static_cast<int>(i));
}

glm::vec3 BezierCurve::evaluate(float u) const {
    if (controlPoints.empty()) {
        return glm::vec3(0.0f);
    }

    unsigned int n = static_cast<unsigned int>(controlPoints.size() - 1);
    glm::vec3 point(0.0f);

    for (unsigned int i = 0; i <= n; ++i) {
        float basis = bernstein(i, n, u);
        point += basis * controlPoints[i];
    }

    return point;
}

std::vector<glm::vec3> BezierCurve::sampleUniform(int segments) const {
    std::vector<glm::vec3> sampledPoints;

    if (controlPoints.empty() || segments < 1) {
        return sampledPoints;
    }

    sampledPoints.reserve(static_cast<size_t>(segments) + 1);

    for (int k = 0; k <= segments; ++k) {
        float u = static_cast<float>(k) / static_cast<float>(segments);
        sampledPoints.push_back(evaluate(u));
    }

    return sampledPoints;
}
