#version 150
#define M_PI 3.1415926535897932384626433832795

in  vec3 pass_Normal, pass_Position;
out vec4 out_Color;
in vec2 pass_TexCoord;

uniform vec3 planet_colour;
uniform vec3 light_colour;
uniform float light_intensity;
uniform vec3 pass_Camera;
uniform bool switch_appearance;
uniform sampler2D YourTexture;

//inmports for normal mapping
uniform bool use_mapping;
uniform sampler2D MappingTex;

float mapping_intensity = 1.0f;
float shininess = 10.0f;
float ambient_light_intesity = 0.1;
float diffuse_reflection_coefficient = 0.6; 

vec3 mapping_normal(){
   //aplying the normal Map
  //------------------------------------------------------------------------------------------------------------------
  //vec4 map_color = texture(MappingTex, pass_TexCoord); //for debugging reasons
  //For just one planet we can choose just one value for this 

  //change of eye-space pos an TexCoordinate, using dFdx and dFdy (return derivative)
  vec3 viewpoint0 = dFdx(pass_Position.xyz);
  vec3 viewpoint1 = dFdy(pass_Position.xyz);
  vec2 tex0 = dFdx(pass_TexCoord.st);
  vec2 tex1 = dFdy(pass_TexCoord.st);


  //N is normal of the two tangents 
  //all three vectors create tension space
  vec3 N = normalize(pass_Normal);
  vec3 S = normalize(viewpoint0 * tex1.t - viewpoint1 * tex0.t); 
  vec3 T = normalize(viewpoint0 * tex1.s + viewpoint0 * tex0.s); 

  //map tension space onto woldspace
  vec3 tNorm = (texture2D(MappingTex ,pass_TexCoord).xyz * 2.0 - 1.0);
  mat3 tension_verse = mat3(S, T, N);
  return (tension_verse * tNorm);  //mapped normal in worldspace

  //vec3 ST = cross(S, T);
  //------------------------------------------------------------------------------------------------------------------
}

vec3 get_normal(){
  if(use_mapping){
    return normalize(mapping_normal());
  }
  else{
    return normalize(pass_Normal);
  }
}

void main() {
 
  vec3 ambient_colour = vec3(1.0f,1.0f,1.0f);
  vec4 colour_from_tex = texture(YourTexture, pass_TexCoord);


  //this is not normilize
  vec3 ambient = ambient_colour*ambient_light_intesity;

  //normalized before used

  vec3 pos = normalize(pass_Position);

  vec3 phi = light_colour * light_intensity;

  //Y is the position of light, X is the Position of the Fragment of the Object
  //the length of the pass Position is X
  //Y is (0 0 0) so we could leave it just in this case out
  vec3 beta = phi/(4*M_PI*(length(pass_Position))*(length(pass_Position)));

  //this is Cd from the slides, the deffuse color
  vec3 light_direction = normalize(vec3(0, 0, 0)-pass_Position);
  float attenuation = max(dot(get_normal(), light_direction),0.0);
  vec3 diffuse = attenuation * light_colour;
  vec3 diffuse_cel = ceil(attenuation * (light_colour*3))/3;

  //here comes the spectular color, it is kinda triccy
  vec3 view_direction = normalize(pass_Camera - pass_Position);

  //halfway vector is vector halfway between view direction and light direction
  //if halfway vector aligns with normal, the higher the specular 
  vec3 halfway_vector = normalize(light_direction + view_direction);
  float spec = pow(max(dot(get_normal(),halfway_vector), 0.0), shininess*4);
  vec3 specular = light_colour * spec;
  vec3 specular_cel = ceil((light_colour*3) * spec)/3;
  vec3 phong = ambient + beta*(diffuse + specular); 
  vec3 phong_cel = ambient + beta*(diffuse_cel + specular_cel);

  float cel_shade_view = dot(get_normal(), view_direction);

  vec3 colour_change;
  
  if(switch_appearance){    
    if(cel_shade_view > 0.35){
    colour_change = planet_colour;
    }
    else colour_change = vec3(0.3,0.4,0.0); // dehydrated piss
      out_Color = colour_from_tex * vec4(phong_cel, 1.0f)/*vec4(normalize(colour_change)* (phong_cel), 1.0)*/;
    }
  else out_Color = colour_from_tex * vec4(phong, 1.0f)/*vec4( normalize(planet_colour) *(phong), 1.0)*/ ;
}
