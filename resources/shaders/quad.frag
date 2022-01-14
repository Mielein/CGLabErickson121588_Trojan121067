#version 150

out vec4 out_FragColor;
in vec2 pass_TexCoords;
uniform sampler2D screen_texture;


void main ( ) {
    vec4 out_colour = texture(screen_texture, pass_TexCoords);

    out_FragColor = out_colour;
    }