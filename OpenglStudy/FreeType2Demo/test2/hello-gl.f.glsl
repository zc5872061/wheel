#version 110

uniform sampler2D textures[2];

varying vec4 g_color;
varying float fade_factor;
varying vec2 g_texcoord;

void main()
{
//    gl_FragColor = mix(
//        texture2D(textures[0], texcoord),
//        texture2D(textures[1], texcoord),
//        fade_factor
//    );
    gl_FragColor =texture2D(textures[1], g_texcoord);
}
