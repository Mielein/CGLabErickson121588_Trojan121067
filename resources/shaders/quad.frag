#version 150

out vec4 outFragColor;
in vec2 pTexCoords;
uniform sampler2D screen_texture;

uniform bool grayscale_on;
uniform bool mirror_v_on;
uniform bool mirror_h_on;
uniform bool blurr_on;
//uniform bool inverse;


void main ( ) {
    vec4 out_colour = texture(screen_texture, pTexCoords);
/*     if(inverse){
        outFragColor = vec4(vec3(1.0 - out_colour), 1.0);
    } */

    float saturation = (out_colour.r + out_colour.g + out_colour.b)/ 3.0;
    vec4 grayscale_color = vec4(saturation, saturation, saturation, 1.0);
    outFragColor = grayscale_color /* out_colour */;
    }