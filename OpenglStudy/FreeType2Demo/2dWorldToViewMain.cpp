#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

using namespace std;


float rtri = 0.0;
float rrect = 0.0;
const float pi = 3.1415926;

void setWindows(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);
}

void setViewport(GLint left,GLint right,GLint bottom,GLint top)
{
    
    glViewport(left, bottom, right-left, top-bottom);
}

void init(void)
{
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
}

void reshape( int w, int h )
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;
    
    glViewport(0,0,w,h);
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);							//定义矩阵
    glLoadIdentity();										//用恒等矩阵替换当前矩阵
    
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();
    gluPerspective(45.0,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
    
    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();
}

void display( void )
{
    glClearColor(0.0f , 0.0f , 0.6f , 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    //glLoadIdentity();
    //glFlush();
    glLoadIdentity();									// Reset The Current Modelview Matrix
    glTranslatef(-1.5f,0.0f,-10.0f);						// Move Left 1.5 Units And Into The Screen 6.0
    glRotatef(rtri, 0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);				// start drawing a polygon
        //glColor3f(0.0f,0.0f,0.0f);			// Set The Color To Red
        glVertex3f( 0.0f, 0.0f, 0.0f);		// Top
        glColor3f(1.0f,1.0f,0.0f);			// Set The Color To Green
        glVertex3f( 0.0f,1.0f, 0.0f);		// Bottom Right
        glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
        glVertex3f(1.0f,1.0f, 0.0f);		// Bottom Left
        glVertex3f(1.0f,0.0f, 0.0f);
    glEnd();
//    glBegin(GL_TRIANGLES);								// Drawing Using Triangles
//    glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
//    glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
//    glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
//    glEnd();    // Finished Drawing The Triangle
    glLoadIdentity();
    glTranslatef(3.0f,0.0f,-10.0f);						// Move Right 3 Units
    glRotatef(rrect, 1.0, 1.0, 0.0);
    glBegin(GL_QUADS);									// Draw A Quad
    glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
    glVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
    glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
    glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
    glEnd();
    glFlush();
    rtri+=1.0f;					// Increase The Rotation Variable For The Triangle
    rrect-=1.0f;
}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
    switch ( key ) {
        case 27:        // When Escape Is Pressed...
            exit ( 0 );   // Exit The Program
            break;        // Ready For Next Case
        default:        // Now Wrap It Up
            break;
    }
}

void arrow_keys ( int a_keys, int x, int y )   // Create Special Function (required for arrow keys)
{
    switch ( a_keys ) {
        case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
            glutFullScreen ( ); // Go Into Full Screen Mode
            break;
        case GLUT_KEY_DOWN:                 // When Down Arrow Is Pressed...
            glutReshapeWindow ( 900, 500 );   // Go Into A 500 By 500 Window
            break;
        default:
            break;
    }
}


void myDisplay(void)
{
    //glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f , 0.0f , 0.6f , 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glColor3f (255.0,1.0,0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin (GL_LINES);
    glVertex2i (120,15);
    glVertex2i (10,145);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 500);
    glVertex2i(900, 0);
    glEnd();
    glFlush();
    //glutSwapBuffers();
}

int main(int argc,char * argv[])
{
    glutInit( &argc, argv );									   //用于初始化GULT库
    //glutInitDisplayMode ( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );  //设置初始显示模式
    glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA);
    glutInitWindowPosition (0,0);								   //确定窗口的位置
    glutInitWindowSize  ( 900, 500 );							   //设置窗口大小
    glutCreateWindow    ( "NeHe Lesson 6- Ported by Rustad" );	   //创建一个窗口标题
    init ();
    //glutFullScreen      ( );          //Put Into Full Screen
    glutDisplayFunc     ( display );	//注册一个绘图函数
    glutReshapeFunc     ( reshape );	//设置投影的函数
    glutKeyboardFunc    ( keyboard );	//键盘回调函数
    glutSpecialFunc     ( arrow_keys );	//设置当前窗口的特殊键盘回调。
    glutIdleFunc    ( display );		//程序空闲时调用的函数
    glutMainLoop        ( );			// 进入GLUT事件处理循环
    
    return 0;
    
}