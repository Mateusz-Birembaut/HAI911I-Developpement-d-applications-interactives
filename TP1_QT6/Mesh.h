#ifndef MESH_H
#define MESH_H

#include "Vec3.h"

#include<QOpenGLBuffer>
#include<QOpenGLVertexArrayObject>
#include <filesystem>
#include <vector>

struct Vertices
{
    Vec3<float> position;
    Vec3<float> normal;
    Vec3<float> color;
};

class Mesh{
public:
    void createRectangleMesh(float length, float width, float height, const Vec3<float>& center);
    void loadFile(const std::filesystem::path& filePath);
    void draw();
    void computeNormals();

    std::vector<Vertices> m_vertices;
    std::vector<unsigned int> m_indices;

private:

    void setupBuffers();
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    bool m_buffersCreated = false;

};

#endif // MESH_H
