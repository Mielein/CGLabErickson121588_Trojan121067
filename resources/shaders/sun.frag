#version 150

in vec3 pass_Normal;
in vec2 pass_TexCoords;
out vec4 out_Color;
uniform sampler2D YourTexture;

vec4 texture_color = texture(YourTexture, pass_TexCoords); 

void main() {
  out_Color = texture_color;
}