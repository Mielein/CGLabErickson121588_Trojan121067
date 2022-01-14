#version 150

out vec4 outFragColor;
in vec2 pTexCoords;
uniform sampler2D screen_texture;

uniform bool grayscale_on;
uniform bool mirror_v_on;
uniform bool mirror_h_on;
uniform bool blurr_on;
uniform bool inverse;

vec2 passCoords = pTexCoords;

void main(){ 

    if(mirror_v_on){
        passCoords.y = 1.0 - passCoords.y;
    }
    if(mirror_h_on){
        passCoords.x = 1.0 - passCoords.x;
    }
    if(blurr_on){

    }

// color 
//----------------------------------------------------------------------------------------
    outFragColor = texture(screen_texture, passCoords);
    if(inverse){
        outFragColor = vec4(vec3(1.0 - outFragColor), 1.0);
    }
    if(grayscale_on){
        float saturation = (outFragColor.r + outFragColor.g + outFragColor.b)/ 3.0;
        vec4 grayscale_color = vec4(saturation, saturation, saturation, 1.0);
        outFragColor = grayscale_color /* out_colour */;
    }
}