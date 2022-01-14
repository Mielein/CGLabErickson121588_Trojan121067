#version 150

out vec4 outFragColor;
in vec2 pTexCoords;
uniform sampler2D screen_texture;

uniform bool grayscale_on;
uniform bool mirror_v_on;
uniform bool mirror_h_on;
uniform bool blurr_on;
uniform bool inverse;


void main(){
    
    outFragColor = texture(screen_texture, pTexCoords);

    if(inverse){
        outFragColor = vec4(vec3(1.0 - outFragColor), 1.0);
    }
    if(grayscale_on){
        float saturation = (outFragColor.r + outFragColor.g + outFragColor.b)/ 3.0;
        vec4 grayscale_color = vec4(saturation, saturation, saturation, 1.0);
        outFragColor = grayscale_color /* out_colour */;
    }
}