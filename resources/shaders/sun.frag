#version 150

in vec3 pass_Normal;
in vec3 pass_TexCoords;
out vec4 out_Color;

uniform sampler2D YourTexture;
void main() {
  out_Color = texture(YourTexture, pass_TexCoords);
}