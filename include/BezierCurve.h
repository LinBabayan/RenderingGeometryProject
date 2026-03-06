#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <vector>
#include <glm/glm.hpp>

class BezierCurve {
public:
    BezierCurve();
    BezierCurve(const std::vector<glm::vec3>& controlPts);

    void setControlPoints(const std::vector<glm::vec3>& controlPts);
    const std::vector<glm::vec3>& getControlPoints() const;

    glm::vec3 evaluate(float u) const;
    std::vector<glm::vec3> sampleUniform(int segments) const;

private:
    std::vector<glm::vec3> controlPoints;

    unsigned int binomialCoefficient(unsigned int n, unsigned int k) const;
    float bernstein(unsigned int i, unsigned int n, float u) const;
};

#endif
