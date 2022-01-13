#version 150

in vec3 pass_Color;
out vec4 out_Color;


uniform sampler2D texture;

void main ( ) {
    out_Color = vec4(texture, pass_Color);
}