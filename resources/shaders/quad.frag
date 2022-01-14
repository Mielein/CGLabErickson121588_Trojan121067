#version 150

out vec4 outFragColor;
in vec2 pTexCoords;
uniform sampler2D screen_texture;

uniform bool inverse;


void main ( ) {
    vec4 out_colour = texture(screen_texture, pTexCoords);
/*     if(inverse){
        outFragColor = vec4(vec3(1.0 - out_colour), 1.0);
    } */

    outFragColor = out_colour;
    }