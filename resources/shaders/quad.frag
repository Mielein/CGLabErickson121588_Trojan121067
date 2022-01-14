#version 150

out vec4 outFragColor;
in vec2 pTexCoords;
uniform sampler2D screen_texture;


void main ( ) {
    vec4 out_colour = texture(screen_texture, pTexCoords);

    outFragColor = out_colour;
    }