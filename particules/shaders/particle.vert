#version 430
layout (location=0) in vec3 position;

uniform mat4 mvp;

// Si utilisation du temps
uniform float time = 0;

out vec3 initialVertPos;

void main(void)
{
    // Position 3D gardée sur le GPU et envoyée au geometry shader
    initialVertPos = position + vec3(0, 0, -0.05) * time;
    
    // Passer la position non transformée au geometry shader
    gl_Position = vec4(initialVertPos, 1.0);
}