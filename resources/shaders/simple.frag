#version 150

in  vec3 pass_Normal;
out vec4 out_Color;


uniform vec3 planet_colour;
uniform uint lightIntensity;

void main() {

  out_Color = vec4(abs(normalize(/* pass_Normal */planet_colour)), 1.0);
}
