#ifndef MESH_H
#define MESH_H

#include "Vec3.h"

#include<QOpenGLBuffer>
#include<QOpenGLVertexArrayObject>
#include <vector>

struct Vertices
{
    Vec3<float> position;
    Vec3<float> normal;
    Vec3<float> color;
};

class Mesh{
public:

    Mesh() {createRectangleMesh(0.5f, 0.25f, 0.5f, {0.0f,0.0f,0.0f});}

    void createRectangleMesh(float length, float width, float height, const Vec3<float>& center);
    void draw();
    void computeNormals();

    std::vector<Vertices> m_vertices;
    std::vector<unsigned int> m_indices;

    void destroyBuffers();
    void setupBuffers();

private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    bool m_buffersCreated = false;

};

#endif // MESH_H
