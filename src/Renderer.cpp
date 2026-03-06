#include "Renderer.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::drawLitMesh(
    Shader& shader,
    Mesh& mesh,
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& objectColor,
    const glm::vec3& lightColor,
    const glm::vec3& lightPos,
    const glm::vec3& viewPos
) {
    shader.use();

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    shader.setVec3("objectColor", objectColor);
    shader.setVec3("lightColor", lightColor);
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", viewPos);

    mesh.draw();
}

void Renderer::drawLineMesh(
    Shader& shader,
    Mesh& mesh,
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& color
) {
    shader.use();

    glm::mat4 mvp = projection * view * model;
    shader.setMat4("mvp", mvp);
    shader.setVec3("lineColor", color);

    mesh.draw();
}

void Renderer::drawPointMesh(
    Shader& shader,
    Mesh& mesh,
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& color,
    float pointSize
) {
    glPointSize(pointSize);
    drawLineMesh(shader, mesh, model, view, projection, color);
}
