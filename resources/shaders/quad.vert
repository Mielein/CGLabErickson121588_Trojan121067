#version 150
#extension GL_ARB_explicit_attrib_location : require
layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_TexCoords;

out vec2 pass_TexCoords;

void main() {
	gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
	pass_TexCoords = in_TexCoords;
}