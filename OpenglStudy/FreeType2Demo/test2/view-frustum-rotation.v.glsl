#version 110

uniform float timer;

attribute vec4 position;
attribute vec4 color;
attribute vec2 texcoord;

varying float fade_factor;
varying vec4 g_color;
varying vec2 g_texcoord;

mat4 view_frustum(
    float angle_of_view,
    float aspect_ratio,
    float z_near,
    float z_far
){
    return mat4(
        vec4(1.0/tan(angle_of_view),           0.0, 0.0, 0.0),
        vec4(0.0, aspect_ratio/tan(angle_of_view),  0.0, 0.0),
        vec4(0.0, 0.0,    (z_far+z_near)/(z_far-z_near), -2.0*z_far*z_near/(z_far-z_near)),
        vec4(0.0, 0.0,-1.0 , 0.0)
    );
    
}

mat4 calculatePerspective(float fov,float aspect,float z_near,float z_far)
{
    float sinValue,cotValue,deltaZ;
    float radiansValue = (fov/2.0)*3.1415926/180.0;
    deltaZ = z_far - z_near;
    sinValue = sin(radiansValue);
    cotValue = cos(radiansValue)/sinValue;
    
    return mat4(
            vec4(cotValue / aspect,0.0,0.0,0.0),
            vec4(0.0,cotValue,0.0,0.0),
            vec4(0.0,0.0,-(z_far + z_near)/deltaZ,-1.0),
            vec4(0.0,0.0,-2.0*z_near*z_far/deltaZ,0.0)
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
    vec3 forwardNormalize = normalize(centPos - eyePos);
    
    vec3 rightNormalize = normalize(cross(forwardNormalize,up));
    vec3 upNormalize = normalize(cross(rightNormalize,forwardNormalize));
    
    vec3 back = vec3(0.0,0.0,0.0) - forwardNormalize;
    
    mat4 viewMatInverse = mat4(
                               vec4(rightNormalize,0.0),
                               vec4(upNormalize,0.0),
                               vec4(back,0.0),
                               vec4(eyePos,1.0)
                               );
    
    float x = -(viewMatInverse[3][0]*viewMatInverse[0][0] + viewMatInverse[3][1]*viewMatInverse[0][1] + viewMatInverse[3][2]*viewMatInverse[0][2]);
    float y = -(viewMatInverse[3][0]*viewMatInverse[1][0] + viewMatInverse[3][1]*viewMatInverse[1][1] + viewMatInverse[3][2]*viewMatInverse[1][2]);
    float z = -(viewMatInverse[3][0]*viewMatInverse[2][0] + viewMatInverse[3][1]*viewMatInverse[2][1] + viewMatInverse[3][2]*viewMatInverse[2][2]);
    
    return mat4(
                vec4(viewMatInverse[0][0],viewMatInverse[1][0],viewMatInverse[2][0],0.0),
                vec4(viewMatInverse[0][1],viewMatInverse[1][1],viewMatInverse[2][1],0.0),
                vec4(viewMatInverse[0][2],viewMatInverse[1][2],viewMatInverse[2][2],0.0),
                vec4(x,y,z,1.0)
                );
}

mat4 orth_matrix(
    float left,
    float right,
    float bottom,
    float top,
    float near,
    float far)
{
    return mat4(
        vec4(2.0/(right - left),0.0,0.0,0.0),
        vec4(0.0,2.0/(top - bottom),0.0,0.0),
        vec4(0.0,0.0,-2.0/(far - near),0.0),
        vec4(-1.0*(right + left)/(right - left),-1.0*(top + bottom)/(top - bottom),-1.0*(far + near)/(far - near),1.0)
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
        vec4(theta,         0.0,         0.0, 0.0),
        vec4(0.0,  cos(timer),  sin(timer), 0.0),
        vec4(0.0, -sin(timer),  cos(timer), 0.0),
        vec4(0.0,         0.0,         0.0, 1.0)
    );
}

void main()
{
    //gl_Position = orth_matrix(0.0,400.0,400.0,0.0, -10.0, 5.0)
        //* translate(cos(timer), 0.0, 3.0+sin(timer))
        //* rotate_x(timer)
        //* scale(1.0/1.0, 1.0, 1.0)
        //* position;
    //mat4 my_perspective = view_frustum(radians(45.0), 4.0/3.0, -5.0, 50.0);
    mat4 my_perspective = calculatePerspective(70.0, 4.0/3.0, 4.0, 1500.0);
    mat4 my_viewMatrix = view_matrix(-10.0,10.0,5.0,  0.0,0.0,0.0,  0.0,1.0,0.0);
    mat4 my_orth_matrix = orth_matrix(0.0,800.0,0.0,600.0, -1.0, 1.5);
    
    gl_Position = my_perspective*my_viewMatrix*position;
    //texcoord = position.xy/10.0 * vec2(0.5) + vec2(0.5);
    fade_factor = sin(timer) * 0.5 + 0.5;
    g_color = color;
    g_texcoord = texcoord;
}
