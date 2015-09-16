#version 430
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

out vec3 colorG;
out vec3 worldPosG;

uniform mat4 PVWMatrix;

void main () {
	colorG = vertex_color;
	//worldPosG = (model * vec4(vertex_position, 1.0)).xyz;
	//gl_Position = projection * view * model * vec4 (vertex_position, 1.0);
	gl_Position = PVWMatrix * vec4 (vertex_position, 1.0);
	worldPosG = vertex_position;
	
	
	//gl_Position = PVWMatrix * vec4 (vertex_position.x, 4.0, 4.0, 1.0);
	//worldPosG = vec3 (vertex_position.x, 4.0, 4.0);
}