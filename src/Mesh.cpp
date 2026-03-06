#include "Mesh.h"

Mesh::Mesh()
    : VAO(0), VBO(0), vertexCount(0), floatsPerVertex(0), drawMode(GL_TRIANGLES) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

Mesh::~Mesh() {
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
}

void Mesh::upload(const std::vector<float>& vertexData, int floatsPerVertex_, GLenum usage) {
    floatsPerVertex = floatsPerVertex_;
    vertexCount = static_cast<int>(vertexData.size()) / floatsPerVertex;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertexData.size() * sizeof(float)),
                 vertexData.data(),
                 usage);
}

void Mesh::update(const std::vector<float>& vertexData, GLenum usage) {
    if (floatsPerVertex <= 0) {
        return;
    }

    vertexCount = static_cast<int>(vertexData.size()) / floatsPerVertex;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertexData.size() * sizeof(float)),
                 vertexData.data(),
                 usage);
}

void Mesh::setAttribute(int index, int size, int stride, int offset) {
    glVertexAttribPointer(index,
                          size,
                          GL_FLOAT,
                          GL_FALSE,
                          stride * static_cast<int>(sizeof(float)),
                          reinterpret_cast<void*>(static_cast<intptr_t>(offset * sizeof(float))));
    glEnableVertexAttribArray(index);
}

void Mesh::setDrawMode(GLenum mode) {
    drawMode = mode;
}

void Mesh::setVertexCount(int count) {
    vertexCount = count;
}

void Mesh::bind() const {
    glBindVertexArray(VAO);
}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(drawMode, 0, vertexCount);
}

unsigned int Mesh::getVAO() const {
    return VAO;
}

int Mesh::getVertexCount() const {
    return vertexCount;
}
