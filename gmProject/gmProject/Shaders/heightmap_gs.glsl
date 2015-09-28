#version 400
layout (triangles) in;
layout (line_strip) out;
layout (max_vertices = 3) out;

in vec3 colorG[];

out vec3 color;

void main () {
	for( int i = 0; i < 3; i++ )
	{
		gl_Position = gl_in[i].gl_Position;
		color = colorG[i];
		EmitVertex();
	}
	EndPrimitive();
}