#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_normal;
layout(location = 2) in vec3 vertex_normal;

out vec2 texture_normal_VS;

uniform mat4 PVWMatrix;

void main () {
	texture_normal_VS = texture_normal;
	gl_Position = PVWMatrix * vec4 (vertex_position, 1.0);
}