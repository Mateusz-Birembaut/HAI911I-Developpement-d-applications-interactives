#ifndef SYSTEMWINDOW_H
#define SYSTEMWINDOW_H

#include "openglwindow.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QElapsedTimer>

#include "particle.h"

class ParticleSystemWindow : public OpenGLWindow {
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;
    ~ParticleSystemWindow()
    {
        delete program;
        delete computeProgram;
        delete smokeTexture;
    }


private:
    std::vector<Particle> particles;
    std::vector<GLfloat> particlePositions;

    QOpenGLBuffer vbo;
    GLuint ssbo = 0;  // Shader Storage Buffer Object pour les particules

    QOpenGLShaderProgram* program = nullptr;
    QOpenGLShaderProgram* computeProgram = nullptr;  // Programme pour le compute shader
    QOpenGLTexture* smokeTexture = nullptr;

    GLint matrixUniform = -1;

    const int numParticles = 2000;
    
    // Gestion du temps
    QElapsedTimer time;
    qint64 lastTime = 0;

};


#endif // SYSTEMWINDOW_H
