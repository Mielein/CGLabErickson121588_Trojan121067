#version 150

out vec4 sky_Color;
in vec3 sky_Pos;

uniform samplerCube skybox;

void main() {
    sky_Color = texture(skybox, sky_Pos);
}