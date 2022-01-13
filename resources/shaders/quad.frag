#version 150

in vec2 pass_TexCoords;
out vec4 out_FragColor;


uniform sampler2D screen_texture;


void main ( ) {
    vec4 texture_colour = texture(screen_texture, pass_TexCoords);
    out_FragColor = texture_colour;
}