#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>

using namespace std;

int motionx = 0;
int motiony = 0;
bool xmoved = false;
bool ymoved = false;

int altura = 0;
int anchura = 0;
int posx =0;
int posy = 0;

GLdouble anglex = 0.0f;
GLdouble angley = 0.0f;

void drawLines(){
    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,0.0,1.0);
    glEnd();

    glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,1.0,0.0);
    glEnd();

    glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(1.0,0.0,0.0);
    glEnd();
    glPopMatrix();
}

void refresh (void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawLines();
    glLoadIdentity(); // Para evitar que se mueva al redimensionar la ventana
    glRotated(angley, 1.0f,0.0f,0.0f);
    glRotated(anglex, 0.0f,1.0f,0.0f);
    glColor3f(0.2,0.4,0.5);
    glutWireTeapot(0.5);
    glutSwapBuffers();
    glutPostRedisplay();
}

void resizeWindow(int width, int height){

   int x = 0;
   int y = 0;
   if(height > width) {
       y = (height - width)/2;
       height = width;
   }
   else {
       x = (width - height)/2;
       width = height;
   }
    altura = height;
    anchura = width;
    posx = x;
    posy = y;
   glViewport(x, y,width , height);
}


void motionFunction(int x, int y){
    cout << x << " " << y << endl;
    cout << anglex << " " << angley << "<- ANGLE" << endl;

    if (x - 5 > motionx) {
        anglex += 2.0;
        motionx = x;
        if(anglex >= 360) anglex = 0.0;

    }
    else if(x + 5 <= motionx ){
        motionx = x;
        anglex -= 2.0;
        if(anglex <= -360) anglex = 0.0;

    }
    if (y - 5 > motiony ) {
        angley += 2.0f;
        motiony = y;
        if(angley >= 360) angley = 0.0;

    }
    else if(y + 5 <= motiony ){
        motiony = y;
        angley -= 2;
        if(angley <= -360) angley = 0.0;
    }
}

void mouseFunction(int key, int keyPress, int x, int y){

}

void keyboardFunc(unsigned char key, int x, int y){

}

void initGL(int argc, char *argv[]){

    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600,600);
    glutCreateWindow("IDI: Practiques OpenGL");
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunction);
    glutMotionFunc(motionFunction);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(refresh);

    //Define el volumen de vision que queremos ( Hace que eje z este afuera)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);

    glutMainLoop();
}

int main(int argc, char * argv[]) {
    initGL(argc, argv);
    return 0;
}
