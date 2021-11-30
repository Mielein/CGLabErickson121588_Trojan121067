#version 150
#define M_PI 3.1415926535897932384626433832795

in  vec3 pass_Normal, pass_Position;
out vec4 out_Color;

uniform vec3 planet_colour;
uniform vec3 light_colour;
uniform float light_intensity;
uniform vec3 pass_Camera;

// all of these is not Phong i think

vec3 ambient_colour = vec3(1.0f,1.0f,1.0f);

float ambient_light_intesity = 0.1;
float diffuse_reflection_coefficient = 0.6; 

//this is not normilize
vec3 ambient = ambient_colour*ambient_light_intesity;

//normalized before used
vec3 normal = normalize(pass_Normal);
vec3 pos = normalize(pass_Position);

vec3 phi = light_colour * light_intensity;

//Y is the position of light, X is the Position of the Fragment of the Object
//the length of the pass Position is X
//Y is (0 0 0) so we could leave it just in this case out
vec3 beta = phi/(4*M_PI*(length(pass_Position))*(length(pass_Position)));


//this is Cd from the slides, the deffuse color
vec3 light_direction = normalize(vec3(0, 0, 0)-pass_Position);
float attenuation = max(dot(normal, light_direction),0.0);
vec3 diffuse = attenuation * light_colour;

//here comes the spectular color, it is kinda triccy
float shininess = 10.0f;
vec3 view_direction = normalize(pass_Camera - pass_Position);

//halfway vector is vector halfway between view direction and light direction
//if halfway vector aligns with normal, the higher the specular 
vec3 halfway_vector = normalize(light_direction + view_direction);
float spec = pow(max(dot(normal,halfway_vector), 0.0), shininess*4);
vec3 specular = light_colour * spec;
vec3 phong = ambient + beta*(diffuse + specular); 


float cel_shade_view = dot(normal, view_direction);

float colour_change;
  
void main() {

  /* if(cel_shade_view > 0.9){
    colour_change = 0.4;
  }
  else if(cel_shade_view > 0.8){
    colour_change = 0.8;
  }
  else if(cel_shade_view > 0.7){
    colour_change = 0.7;
  }
  else if(cel_shade_view > 0.6){
    colour_change = 0.6;
  }
  else if(cel_shade_view > 0.5){
    colour_change = 0.5;
  }
  else if(cel_shade_view > 0.4){
    colour_change = 0.4;
  }
  else if(cel_shade_view > 0.3){
    colour_change = 0.3;
  }
  else if(cel_shade_view > 0.2){
    colour_change = 0.2;
  } */
  if(cel_shade_view > 0.3){
    colour_change = 1.0;
  }
  else colour_change = 0.4;
  out_Color = vec4(normalize(planet_colour)*(phong)*colour_change, 1.0);
  
}
