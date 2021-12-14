#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec3 in_TexCoords;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
//Matrix Uniforms as specified with glUniformMatrix4fv
out vec3 sky_Pos;
out vec3 gl_Position;

void main(void)
{
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(in_Position, 1.0);
    sky_Pos = in_Position
}