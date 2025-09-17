#version 150
in vec3 vertex;
in vec3 normal;
in vec3 color;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_color;

uniform mat4 mvp_matrix;
uniform mat3 normal_matrix;
void main() {
    v_position = vertex;
    v_normal = normal_matrix * normal;
    v_color = color;

    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(vertex, 1.0);
}
