#version 150

out vec4 outFragColor;
in vec2 pTexCoords;
uniform sampler2D screen_texture;

uniform bool grayscale_on;
uniform bool mirror_v_on;
uniform bool mirror_h_on;
uniform bool blur_on;
uniform bool inverse;

vec2 passCoords = pTexCoords;

void main(){ 

    if(mirror_v_on){
        passCoords.y = 1.0 - passCoords.y;
    }
    if(mirror_h_on){
        passCoords.x = 1.0 - passCoords.x;
    }

// color 
//----------------------------------------------------------------------------------------
    if(!blur_on){
        outFragColor = texture(screen_texture, passCoords);
    }
    else{
        float blur_strength = 1.0/300.0;

        vec2 blur_strength_location[9] = vec2[](
            vec2(-blur_strength, blur_strength),     //  [->...->
            vec2( 0.0f, blur_strength),
            vec2( blur_strength,  blur_strength),    
            vec2(-blur_strength,  0.0f),            //    ->...->
            vec2( 0.0f,    0.0f),  
            vec2( blur_strength,  0.0f), 
            vec2(-blur_strength, -blur_strength),   //    ->...->]
            vec2( 0.0f,   -blur_strength), 
            vec2( blur_strength, -blur_strength)     
        );
        float blur_kernel[9] = float[](
            1.0/16, 1.0/8,    1.0/16,
            1.0/8,  1.0/4,    1.0/8,
            1.0/16, 1.0/8,    1.0/16 
        );
        
        vec3 tex_color;
        for(int i=0; i<9; i++){
            tex_color += vec3(texture(screen_texture, passCoords.xy + blur_strength_location[i]))*blur_kernel[i];
        }
        outFragColor = vec4(tex_color, 1.0);
    }

    if(inverse){
        outFragColor = vec4(vec3(1.0 - outFragColor), 1.0);
    }
    if(grayscale_on){
        float luminance = (0.2126 * outFragColor.r + 0.7152 * outFragColor.g + 0.7152 * outFragColor.b)/ 3.0;
        vec4 grayscale_color = vec4(luminance, luminance, luminance, 1.0);
        outFragColor = grayscale_color /* out_colour */;
    }
}