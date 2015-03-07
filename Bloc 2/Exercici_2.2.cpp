#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
#include "model.h"
using namespace std;

int motionx = 0;
int motiony = 0;
int altura = 0;
int anchura = 0;
int posx =0;
int posy = 0;

GLdouble anglex = 0.0;
GLdouble angley = 0.0;

bool camera = true;

GLdouble tx = 0.0; 
GLdouble ty = 0.0;
bool trans = false;

GLdouble translatex = 0.0;
GLdouble translatey = 0.0;

GLdouble xcen, ycen, zcen,maximum;
GLdouble ymin, xmin, zmin, ymax,xmax,zmax;
Model model;

double max(double i, double j){
    return (i > j ) ? i : j;
}

double min(double i, double j){
    return (i > j ) ? j : i;
}

void drawSnowMan(){
    //CONO
    glPushMatrix();

    glTranslatef(0.1,0.6,0.0);
    glRotatef(90.0,0.0,1.0,0.0);

    glColor4f(1.0,0.0,0.0,1.0);
    glutWireCone(0.1,0.2,50,50);

    glPopMatrix();

    //ESFERA GRANDE
    glPushMatrix();

    glColor4f(1.0,1.0,1.0,0.6);
    glutWireSphere(0.4,50,50);
    
    glPopMatrix();
  
    //ESFERA PEQUEÑA
    glPushMatrix();
    
    glTranslatef(0.0,0.6,0.0);
   
    glColor4f(1.0,1.0,1.0,0.6);
    glutWireSphere(0.2,50,50);
    glPopMatrix();

}

void setCenterPoints(){

    xmin = xmax = model.vertices()[0];
    ymin = ymax = model.vertices()[1];
    zmin = zmax = model.vertices()[2];

    for(int i = 0; i < model.vertices().size(); i += 3){
        xmax = max(xmax, model.vertices()[i]);
        xmin = min(xmin, model.vertices()[i]);

        ymax = max(ymax, model.vertices()[i+1]);
        ymin = min(ymin, model.vertices()[i+1]);

        zmax = max(zmax, model.vertices()[i+2]);
        zmin = min(zmin, model.vertices()[i+2]);

    }
    xcen =  xmin + (xmax - xmin) / 2 ;
    ycen =  ymin + (ymax - ymin) /2; 
    zcen =  zmin + (zmax - zmin) / 2 ;
 
    maximum = max(xmax - xmin , ymax - ymin);
    maximum = max(maximum, zmax - zmin);

}
void drawHomer(){

    glPushMatrix();

    glTranslated(-0.75,-0.4,0.75);
    glScaled((0.5/maximum), (0.5/maximum), (0.5/maximum));
    glTranslated(-xmin,-ymin,-zmax);
    glTranslatef(tx,ty,0.0);

    glBegin(GL_TRIANGLES);
        for(int i = 0; i < model.faces().size(); i++){
            const Face &f = model.faces()[i];
            for(int j = 0; j < 3; j++){
                glColor4fv(Materials[f.mat].diffuse);
                glVertex3dv(&model.vertices()[f.v[j]]);
            }
        }
    glEnd();

    glPopMatrix();
}

void drawLinesAndGround(){

    glBegin(GL_QUADS);
        glColor4f(0.5,0.5,0.0,0.4);
        glVertex3f(0.75,-0.4,0.75);
        glVertex3f(0.75,-0.4,-0.75);
        glVertex3f(-0.75 ,-0.4,-0.75);
        glVertex3f(-0.75,-0.4,0.75);
    glEnd();
   
    glBegin(GL_LINES);
        glColor3f(0.0,1.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.0,1.0);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,1.0,0.0);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(1.0,0.0,0.0);
    glEnd();
   
    glColor3f(1.0,1.0,1.0);
}

void refresh (void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawLinesAndGround();
    setCenterPoints();
    drawHomer();
    drawSnowMan();
    glLoadIdentity();
    glRotated(angley, 1.0f,0.0f,0.0f);
    glRotated(anglex, 0.0f,1.0f,0.0f);
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

    if(camera){
        if (x - 5 > motionx) {
            anglex +=2.0;
            motionx = x;
            if(anglex >= 360) anglex = 0.0;
        }
        else if(x + 5 <= motionx ){
            motionx = x;
            anglex -=2.0;
            if(anglex <= -360) anglex = 0.0;

        }
        if (y - 5 > motiony ) {
            angley +=2.0;
            motiony = y;
            if(angley >= 360) angley = 0.0;

        }
        else if(y + 5 <= motiony ){
            motiony = y;
            angley -=2.0;
            if(angley <= -360) angley = 0.0;
        }
    }
    else if(trans){
         if (x - 5 > translatex) {
            translatex = x;
            tx += 15;
            
        }
        else if(x + 5 <= translatex ){
            translatex = x;
            tx -= 15;
        }
        if (y - 5 > translatey ) {
            ty -= 15;
            translatey = y;
        }
        else if(y + 5 <= translatey ){
            translatey = y;
            ty +=15;
        }
    }
}

void mouseFunction(int key, int keyPress, int x, int y){}

void keyboardFunc(unsigned char key, int x, int y){
  if(key == 'c'){
    trans = true;
    camera = false;
  }
  else if(key == 27){
    exit(0);
  }
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
    model.load("./Model/legoman.obj");
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