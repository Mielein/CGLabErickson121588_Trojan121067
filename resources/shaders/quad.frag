#version 150

in vec2 TexCoords;
out vec4 out_Color;


uniform sampler2D screen_texture;


void main ( ) {
    vec4 texture_colour = texture(screen_texture, TexCoords);
    out_Color = texture_colour;
}