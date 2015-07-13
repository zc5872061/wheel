#version 110

uniform float timer;

attribute vec4 position;

varying vec2 texcoord;
varying float fade_factor;

mat4 view_frustum(
    float angle_of_view,
    float aspect_ratio,
    float z_near,
    float z_far
) {
    return mat4(
        vec4(1.0/tan(angle_of_view),           0.0, 0.0, 0.0),
        vec4(0.0, aspect_ratio/tan(angle_of_view),  0.0, 0.0),
        vec4(0.0, 0.0,    (z_far+z_near)/(z_far-z_near), 1.0),
        vec4(0.0, 0.0, -2.0*z_far*z_near/(z_far-z_near), 0.0)
    );
}


mat4 view_matrix(
            float eyePosx,
            float eyePosy,
            float eyePosz,
            float centerx,
            float centery,
            float centerz,
            float upx,
            float upy,
            float upz)
{
    vec3 eyePos = vec3(eyePosx,eyePosy,eyePosz);
    vec3 centPos = vec3(centerx,centery,centerz);
    
    vec3 up = vec3(upx,upy,upz);
    vec3 direction = normalize(eyePos - centPos);
    
    vec3 rightNormalize = normalize(cross(up,direction));
    vec3 upNormalize = normalize(cross(rightNormalize,direction));
    
    float x = -1.0*dot(centPos,rightNormalize);
    float y = -1.0*dot(centPos,upNormalize);
    float z = -1.0*dot(centPos,direction);
    
    return mat4(
            vec4(rightNormalize.x,upNormalize.x,direction.x,0.0),
            vec4(rightNormalize.y,upNormalize.y,direction.y,0.0),
            vec4(rightNormalize.z,upNormalize.z,direction.z,0.0),
            vec4(x,y,z,1.0)
        );
}

mat4 orth_matrix(
    float right,
    float left,
    float top,
    float bottom,
    float far,
    float near)
{
    return mat4(
        vec4(2.0/(right - left),0.0,0.0,-1.0*(right + left)/(right - left)),
        vec4(0.0,2.0/(top - bottom),0.0,-1.0*(top + bottom)/(top - bottom)),
        vec4(0.0,0.0,-2.0/(far - near),-1.0*(far + near)/(far - near)),
        vec4(0.0,0.0,0.0,1.0)
    );
}

mat4 scale(float x, float y, float z)
{
    return mat4(
        vec4(x,   0.0, 0.0, 0.0),
        vec4(0.0, y,   0.0, 0.0),
        vec4(0.0, 0.0, z,   0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

mat4 translate(float x, float y, float z)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

mat4 rotate_x(float theta)
{
    return mat4(
        vec4(1.0,         0.0,         0.0, 0.0),
        vec4(0.0,  cos(timer),  sin(timer), 0.0),
        vec4(0.0, -sin(timer),  cos(timer), 0.0),
        vec4(0.0,         0.0,         0.0, 1.0)
    );
}

void main()
{
//    gl_Position = view_frustum(radians(45.0), 4.0/3.0, 0.5, 5.0)
//        * translate(cos(timer), 0.0, 3.0+sin(timer))
//        * rotate_x(timer)
//        * scale(1.0/1.0, 1.0, 1.0)
//        * position;
    //view_frustum(radians(45.0), 4.0/3.0, 0.5, 5.0)
    gl_Position = position;
    texcoord = position.xy * vec2(0.5) + vec2(0.5);
    fade_factor = sin(timer) * 0.5 + 0.5;
}
