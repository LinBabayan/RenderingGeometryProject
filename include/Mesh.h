#ifndef MESH_H
#define MESH_H

#include <glad/gl.h>
#include <vector>

class Mesh {
public:
    Mesh();
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void upload(const std::vector<float>& vertexData,
                int floatsPerVertex,
                GLenum usage = GL_STATIC_DRAW);

    void update(const std::vector<float>& vertexData,
                GLenum usage = GL_DYNAMIC_DRAW);

    void setAttribute(int index, int size, int stride, int offset);
    void setDrawMode(GLenum mode);
    void setVertexCount(int count);

    void bind() const;
    void draw() const;

    unsigned int getVAO() const;
    int getVertexCount() const;

private:
    unsigned int VAO;
    unsigned int VBO;
    int vertexCount;
    int floatsPerVertex;
    GLenum drawMode;
};

#endif
