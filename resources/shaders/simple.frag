#version 150

in  vec3 pass_Normal, pass_Position;
out vec4 out_Color;

uniform vec3 planet_colour;
uniform vec3 light_colour;
uniform float light_intensity; //k_a

float diffuse_light_intesity = 0.1; //I_p
float diffuse_reflection_coefficient = 0.5; //k_d

vec3 ambient = light_colour*light_intensity;

vec3 normal = normalize(pass_Normal); //N
float bisector = (dot(normal,pass_Position))/(length(normal)*length(pass_Position)); //cos_theta

float diffuse = light_intensity*diffuse_reflection_coefficient*bisector;

void main() {

  out_Color = vec4(abs(normalize(planet_colour))*(/* ambient +  */diffuse), 1.0);
  
}
