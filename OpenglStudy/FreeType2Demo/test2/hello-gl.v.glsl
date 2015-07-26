#version 110

uniform float timer;

attribute vec4 position;

varying vec2 g_texcoord;

void main()
{
    mat4 viewMat = mat4(
                        vec4(1.0,   0.0, 0.0, 0.0),
                        vec4(0.0, 1.0,   0.0, 0.0),
                        vec4(0.0, 0.0, 1.0,   0.0),
                        vec4(0.0, 0.0, -1.0, 1.0)
                        );
    
    mat4 projMat = mat4(
                        vec4(3.07438993,   0.0, 0.0, 0.0),
                        vec4(0.0, 1.73205078,   0.0, 0.0),
                        vec4(0.0, 0.0, -1.0100503,   -1.0),
                        vec4(0.0, 0.0,-1.00502515, 1.0)
                        );


    gl_Position = projMat*viewMat*position;
    g_texcoord = position.xy * vec2(0.5) + vec2(0.5);
    
}
