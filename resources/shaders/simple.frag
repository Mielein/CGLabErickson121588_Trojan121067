#version 150
#define M_PI 3.1415926535897932384626433832795

in  vec3 pass_Normal, pass_Position;
out vec4 out_Color;

uniform vec3 planet_colour;
uniform vec3 light_colour;
uniform float light_intensity;

// all of these is not Phong i think

vec3 ambient_colour = vec3(1.0f,1.0f,1.0f);

float ambient_light_intesity = 0.2;
float diffuse_reflection_coefficient = 0.9; 

//this is not normilize
vec3 ambient = ambient_colour*ambient_light_intesity;

//normalized before used
vec3 normal = normalize(pass_Normal);
vec3 pos = normalize(pass_Position);

vec3 phi = light_colour * light_intensity;

//Y is the position of light, X is the Position of the Fragment of the Object
//the length of the pass Position is X
//Y is (0 0 0) so we could leave it just in this case out
vec3 beta = phi/(4*M_PI*length(pass_Position)*length(pass_Position));


//this is Cd from the slides, the deffuse color
vec3 light_direction = normalize(normal-pos);
float attenuation = max(dot(normal, light_direction),0.0);

//here comes the spectular color, it is kinda triccy

vec3 phong = ambient + (beta * attenuation); 

void main() {

  out_Color = vec4(normalize(planet_colour)*(phong), 1.0);
  
}
