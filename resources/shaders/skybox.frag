#version 150

in vec3 sky_Pos;
out vec4 sky_Color;

uniform samplerCube skybox;

vec4 texture_color = texture(skybox, sky_Pos); 

void main() {
    sky_Color = texture(skybox, sky_Pos);
}