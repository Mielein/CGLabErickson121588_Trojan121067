#version 150

in  vec3 pass_Normal, pass_Position;
out vec4 out_Color;

uniform vec3 planet_colour;
uniform vec3 light_colour;
uniform float light_intensity; //k_a

vec3 ambient_colour = vec3(1.0f,1.0f,1.0f);

float ambient_light_intesity = 2; //I_a
float diffuse_reflection_coefficient = 0.9; //k_d

vec3 ambient = normalize(ambient_colour*ambient_light_intesity);

vec3 normal = normalize(pass_Normal); //N
vec3 pos = normalize(pass_Position);
float bisector = dot(normal,pos)/(length(normal)*length(pos)); //cos_theta

vec3 light_direction = normalize(normal-pos);

float attenuation = 1/max(dot(normal, light_direction),0.0);

vec3 diffuse = bisector*light_colour*diffuse_reflection_coefficient*attenuation;

void main() {

  out_Color = vec4(abs(normalize(planet_colour))*( ambient  + diffuse ), 1.0);
  
}
