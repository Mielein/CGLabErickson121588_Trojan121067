#version 150

in  vec3 pass_Normal, pass_Position;
out vec4 out_Color;

uniform vec3 planet_colour;
uniform vec3 light_colour;
uniform float light_intensity;

//norm vector from point to viewer
vec3 v = pass_Position;

//normalized vector from point to viewable point light source
vec3 l = normalize(pass_Normal);

//normal
vec3 n = normalize(l);
//bisector
//float h = (v+l)/norm(v+l);

//float theta = cos(n*h);
float diffuse_light_intesity = 0.1;
float diffuse_reflection_coefficient = 0.5;

vec3 ambient = light_colour*light_intensity;
//vec3 diffuse =

void main() {

  out_Color = vec4(abs(normalize(planet_colour))*ambient, 1.0);
  
}
