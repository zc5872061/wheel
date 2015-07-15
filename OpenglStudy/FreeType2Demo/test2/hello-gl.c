#include <stdlib.h>
#include <GL/glew.h>

#include <GLUT/glut.h>

#include <math.h>
#include <stdio.h>
#include "util.h"

/*
 * Global data used by our render callback:
 */

static struct {
    GLuint vertex_buffer,color_buffer,texcood_buffer, element_buffer;
    GLuint textures[2];
    GLuint vertex_shader, fragment_shader, program;
    
    struct {
        GLint timer;
        GLint textures[2];
    } uniforms;

    struct {
        GLint position;
        GLint color;
        GLint texcoord;
    } attributes;

    GLfloat timer;
} g_resources;

/*
 * Functions for creating OpenGL objects:
 */
static GLuint make_buffer(
    GLenum target,
    const void *buffer_data,
    GLsizei buffer_size
) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;
}

static GLuint make_texture(const char *filename)
{
    int width, height;
    void *pixels = read_tga(filename, &width, &height);
    GLuint texture;

    if (!pixels)
        return 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexImage2D(
        GL_TEXTURE_2D, 0,           /* target, level */
        GL_RGB8,                    /* internal format */
        width, height, 0,           /* width, height, border */
        GL_BGR, GL_UNSIGNED_BYTE,   /* external format, type */
        pixels                      /* pixels */
    );
    free(pixels);
    return texture;
}

static void show_info_log(
    GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
)
{
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

static GLuint make_shader(GLenum type, const char *filename)
{
    GLint length;
    GLchar *source = file_contents(filename, &length);
    GLuint shader;
    GLint shader_ok;

    if (!source)
        return 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    free(source);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        fprintf(stderr, "Failed to compile %s:\n", filename);
        show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static GLuint make_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLint program_ok;

    GLuint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        fprintf(stderr, "Failed to link shader program:\n");
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

/*
 * Data used to seed our vertex array and element array buffers:
 */
static const GLfloat g_vertex_buffer_data[] = {
    //front
    -1.0f, -1.0f, -5.0f, 1.0f,
     1.0, -1.0f, -5.0f, 1.0f,
    -1.0f,  1.0, -5.0f, 1.0f,
     1.0,  1.0, -5.0f, 1.0f,
    
    
    //back
    -2.0f, -1.0f, -8.0f, 1.0f,
    2.0, -1.0f, -8.0f, 1.0f,
    -2.0f,  1.0, -8.0f, 1.0f,
    2.0,  1.0, -8.0f, 1.0f,

};

static const GLfloat g_vertex_buffer_color[] = {
    //front
    1.0f, 0.0f,0.0f,1.0f,
    0.0f,1.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,1.0f,
    0.0f,0.0f,0.0f,1.0f,
    
    //back
    1.0f, 0.0f,0.0f,1.0f,
    0.0f,1.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,1.0f,
    0.0f,0.0f,0.0f,1.0f,
//    1.0f, 0.0f,0.0f,1.0f,
//    0.0f,1.0f,0.0f,1.0f,
//    0.0f,0.0f,1.0f,1.0f,
//    0.0f,0.0f,0.0f,1.0f
};

static const GLfloat g_texcoord[] = {
    //front
    0.0f,0.0f,
    1.0f,0.0f,
    0.0f,1.0f,
    1.0f,1.0f,
    //back
    0.0f,0.0f,
    1.0f,0.0f,
    0.0f,1.0f,
    1.0f,1.0f,
};

//static const GLfloat g_vertex_buffer_data[] = {
//    0.0f, 0.0f, 0.0f, 1.0f,
//    100.0, 0.0f, 0.0f, 1.0f,
//    0.0f,  100.0, 0.0f, 1.0f,
//    100.0, 100.0, 0.0f, 1.0f
//};
static const GLushort g_element_buffer_data[] = {0,1,2,1,2,3,4,5,6,5,6,7};

/*
 * Load and create all of our resources:
 */
static int make_resources(const char *vertex_shader_file)
{
    g_resources.vertex_buffer = make_buffer(
        GL_ARRAY_BUFFER,
        g_vertex_buffer_data,
        sizeof(g_vertex_buffer_data)
    );
    
    g_resources.color_buffer = make_buffer(
        GL_ARRAY_BUFFER,
        g_vertex_buffer_color,
        sizeof(g_vertex_buffer_color)
    );
    
    g_resources.texcood_buffer = make_buffer(
        GL_ARRAY_BUFFER,
        g_texcoord,
        sizeof(g_texcoord)
    );
    
    g_resources.element_buffer = make_buffer(
        GL_ELEMENT_ARRAY_BUFFER,
        g_element_buffer_data,
        sizeof(g_element_buffer_data)
    );
    
    
    
    g_resources.textures[0] = make_texture("/Users/chukie/wheel/OpenglStudy/FreeType2Demo/test2/hello1.tga");
    g_resources.textures[1] = make_texture("/Users/chukie/wheel/OpenglStudy/FreeType2Demo/test2/hello2.tga");

    if (g_resources.textures[0] == 0 || g_resources.textures[1] == 0)
        return 0;

    g_resources.vertex_shader = make_shader(
        GL_VERTEX_SHADER,
        vertex_shader_file
    );
    if (g_resources.vertex_shader == 0)
        return 0;

    g_resources.fragment_shader = make_shader(
        GL_FRAGMENT_SHADER,
        "/Users/chukie/wheel/OpenglStudy/FreeType2Demo/test2/hello-gl.f.glsl"
    );
    if (g_resources.fragment_shader == 0)
        return 0;

    g_resources.program = make_program(g_resources.vertex_shader, g_resources.fragment_shader);
    if (g_resources.program == 0)
        return 0;

    g_resources.uniforms.timer
        = glGetUniformLocation(g_resources.program, "timer");
    g_resources.uniforms.textures[0]
        = glGetUniformLocation(g_resources.program, "textures[0]");
    g_resources.uniforms.textures[1]
        = glGetUniformLocation(g_resources.program, "textures[1]");

    g_resources.attributes.position
        = glGetAttribLocation(g_resources.program, "position");
    g_resources.attributes.color
        = glGetAttribLocation(g_resources.program,"color");
    g_resources.attributes.texcoord
        = glGetAttribLocation(g_resources.program,"texcoord");
    return 1;
}


static void update_timer(void)
{
    int milliseconds = glutGet(GLUT_ELAPSED_TIME);
    g_resources.timer = (float)milliseconds * 0.001f;
    //glutPostRedisplay();
}

static void render(void)
{
    glClearColor(0.0f, 0.25f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    
    glDisable(GL_CULL_FACE);
    glUseProgram(g_resources.program);

    glUniform1f(g_resources.uniforms.timer, g_resources.timer);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_resources.textures[0]);
    glUniform1i(g_resources.uniforms.textures[0], 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_resources.textures[1]);
    glUniform1i(g_resources.uniforms.textures[1], 1);
    
    glBindBuffer(GL_ARRAY_BUFFER,g_resources.color_buffer);
    glVertexAttribPointer(g_resources.attributes.color,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GLfloat)*4,
                          (void*)0
    );
    glEnableVertexAttribArray(g_resources.attributes.color);
    
    glBindBuffer(GL_ARRAY_BUFFER,g_resources.texcood_buffer);
    glVertexAttribPointer(g_resources.attributes.texcoord,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GLfloat)*2,
                          (void*)0
                          );
    glEnableVertexAttribArray(g_resources.attributes.texcoord);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, g_resources.vertex_buffer);
    glVertexAttribPointer(
        g_resources.attributes.position,  /* attribute */
        4,                                /* size */
        GL_FLOAT,                         /* type */
        GL_FALSE,                         /* normalized? */
        sizeof(GLfloat)*4,                /* stride */
        (void*)0                          /* array buffer offset */
    );
    glEnableVertexAttribArray(g_resources.attributes.position);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_resources.element_buffer);
    glDrawElements(
        GL_TRIANGLES,  /* mode */
        12,                  /* count */
        GL_UNSIGNED_SHORT,  /* type */
        (void*)0            /* element array buffer offset */
    );

    glDisableVertexAttribArray(g_resources.attributes.position);
    glClear(GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hello World");
    glutIdleFunc(&update_timer);

    
    glutDisplayFunc(&render);
    
    glewInit();
    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 not available\n");
        return 1;
    }

    if (!make_resources(argc >= 2 ? argv[1] : "/Users/chukie/wheel/OpenglStudy/FreeType2Demo/test2/view-frustum-rotation.v.glsl")) {
        fprintf(stderr, "Failed to load resources\n");
        return 1;
    }

    glutMainLoop();
    return 0;
}

