#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

#include "Shader.h"
#include "Mesh.h"

class Renderer {
public:
    Renderer() = default;

    void drawLitMesh(
        Shader& shader,
        Mesh& mesh,
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& objectColor,
        const glm::vec3& lightColor,
        const glm::vec3& lightPos,
        const glm::vec3& viewPos
    );

    void drawLineMesh(
        Shader& shader,
        Mesh& mesh,
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& color
    );

    void drawPointMesh(
        Shader& shader,
        Mesh& mesh,
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& color,
        float pointSize
    );
};

#endif
