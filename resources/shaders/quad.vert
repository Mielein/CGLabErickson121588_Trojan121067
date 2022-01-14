#version 150
#extension GL_ARB_explicit_attrib_location : require
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoords;

out vec2 pTexCoords;

void main() {
	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);
	pTexCoords = inTexCoords;
}