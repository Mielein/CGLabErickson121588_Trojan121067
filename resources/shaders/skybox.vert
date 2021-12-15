#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;


uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
//Matrix Uniforms as specified with glUniformMatrix4fv
out vec3 sky_Pos;

void main(void)
{
    sky_Pos = in_Position;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(in_Position, 1.0);
}