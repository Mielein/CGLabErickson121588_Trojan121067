#version 150

in vec3 pass_Color;
out vec4 out_Color;


uniform sampler2D screen_texture;


void main ( ) {
    vec4 texture_colour = texture(screen_texture, pass_Color);
    out_Color = texture_colour;
}