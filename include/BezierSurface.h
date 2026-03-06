#ifndef BEZIERSURFACE_H
#define BEZIERSURFACE_H

#include <vector>
#include <glm/glm.hpp>

struct SurfaceVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class BezierSurface {
public:
    BezierSurface();
    BezierSurface(const std::vector<std::vector<glm::vec3>>& controlGrid);

    void setControlGrid(const std::vector<std::vector<glm::vec3>>& grid);
    const std::vector<std::vector<glm::vec3>>& getControlGrid() const;

    glm::vec3 evaluate(float u, float v) const;
    glm::vec3 evaluateDu(float u, float v) const;
    glm::vec3 evaluateDv(float u, float v) const;

    std::vector<SurfaceVertex> sampleUniform(int uSegments, int vSegments) const;
    std::vector<float> sampleUniformInterleaved(int uSegments, int vSegments) const;

private:
    std::vector<std::vector<glm::vec3>> controlPoints;

    unsigned int binomialCoefficient(unsigned int n, unsigned int k) const;
    float bernstein(unsigned int i, unsigned int n, float t) const;
    float bernsteinDerivative(unsigned int i, unsigned int n, float t) const;
};

#endif
