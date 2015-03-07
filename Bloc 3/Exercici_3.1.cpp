#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
#include "model.h"
#include <cmath>
using namespace std;

int motionx = 0;
int motiony = 0;
int altura = 0;
int anchura = 0;
int posx =0;
int posy = 0;

GLdouble anglex = 0.0;
GLdouble angley = 0.0;
GLdouble anglez = 0.0;
bool camera = true;

GLdouble tx = 0.0; 
GLdouble ty = 0.0;
bool trans = false;

bool oneClick = true;
bool twoClick = false;

GLdouble translatex = 0.0;
GLdouble translatey = 0.0;
GLdouble xcen, ycen, zcen,maximum;
GLdouble ymin, xmin, zmin, ymax,xmax,zmax;
Model model;

GLdouble FOV = 45.0;
GLdouble axoZoom = 0.0;
struct VRP {
    int x; 
    int y;
    int z;
};

bool perspective = false;
//Variables para calcular esfera contenedora

GLdouble radio = 0.0;

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
    
    glPushMatrix();
    glTranslatef(0.0,0.2,0.0);
    glutWireSphere(radio, 50,50);
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
    glTranslatef(tx,0.0,-ty);

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
        glColor4f(0.5,0.8,0.0,0.4);
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

void ini_pos_camera(){
    cout << anglex << " " << angley << endl;
    glLoadIdentity();
    //glPushMatrix();
        glTranslated(0.0,0.0,-radio);
        glRotated(0.0,0.0,0.0,1.0);
        glRotated(angley, 1.0, 0.0,0.0);
        glRotated(anglex, 0.0,1.0,0.0);
        glTranslated(0.0, 0.0, 0.0); //VRP 
    //glPopMatrix();
}

void refresh (void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
    //glRotated(angley, 1.0f,0.0f,0.0f);
    //glRotated(anglex, 0.0f,1.0f,0.0f);
    drawLinesAndGround();
    setCenterPoints();
    drawHomer();
    drawSnowMan();
    
    glutSwapBuffers();
    glutPostRedisplay();
}


void setRadio(){
  
  GLdouble x1 = 0.75;
  GLdouble y1 = -0.4;
  GLdouble z1 = 0.75;
  
  GLdouble x2 = -0.75;
  GLdouble y2 = 0.8;
  GLdouble z2 = -0.75;
  
  radio = sqrt((x2 - x1)*(x2 - x1) + (y2 -y1) * (y2 -y1) + (z2 - z1) * (z2 - z1));
  radio /= 2.0;
  
}



void resizeWindow(int width, int height){
   /*int x = 0;
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
    posy = y;*/
   glViewport(0, 0, width , height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLdouble ratio = width / double(height); 
   GLdouble top;
   GLdouble left;
   if(ratio >= 1){
        top = radio * ratio;
        left = radio;
   }
   else if(ratio < 1){
        top = radio;
        left = radio / double(ratio);
   }
   if(!perspective) glOrtho(-top - axoZoom,top + axoZoom,-left - axoZoom ,left + axoZoom,-radio,radio*2);
   else gluPerspective(FOV,ratio,0.1f, 2*radio);
   glMatrixMode(GL_MODELVIEW);
}

void motionFunction(int x, int y){

    if(oneClick){
        twoClick = false;
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
        ini_pos_camera();
        resizeWindow(glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT));
    }
    else if(twoClick){
        if (y - 5 > motiony ) {
             motiony = y;
            FOV += 2;
            axoZoom += 0.05;
        }
        else if(y + 5 <= motiony){
            motiony = y;
            FOV-= 2;
            axoZoom -= 0.05;
        }
        cout << axoZoom << " <- axoZoom" << endl;
        resizeWindow(glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT));
    }
    /*else if(twoClick && !perspective){
        if (y - 5 > motiony ) {
             motiony = y;
            axoZoom -= 0.05;
        }
        else if(y + 5 <= motiony){
            motiony = y;
            axoZoom += 0.05;
        }
        resizeWindow(glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT));
    }*/
}

void mouseFunction(int key, int keyPress, int x, int y){
    cout << key << " " << keyPress << " " << x << " " << y << endl;
    if(key == 0){
        oneClick = true;
        twoClick = false;
    }
    else if (key == 2){
        oneClick = false;
        twoClick = true;
    }
}

void keyboardFunc(unsigned char key, int x, int y){
  /*if(key == 'c'){
    trans = true;
    camera = false;
  }*/
  if(key == 'p'){
    cout << "KEY P Pressed" << endl;
    perspective = perspective ^ true;
    resizeWindow(glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT));
  }
  if(key == 'k'){
    cout << "HELP" << endl;
    cout << "Presione tecla k para cambiar de camara (axonometrica o perspectiva)." << endl;
    cout << "Presione la tecla Escape para salir de la aplicacion." << endl;
  }
  if(key == 27){
    exit(0);
  }
}

void initGL(int argc, char *argv[]){

    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("IDI: Practiques OpenGL");

    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunction);
    glutMotionFunc(motionFunction);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(refresh);
    model.load("legoman.obj");
    glEnable(GL_DEPTH_TEST);
    //Define el volumen de vision que queremos ( Hace que eje z este afuera)
    setRadio();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-radio , radio ,-radio ,radio,-radio,radio);
    
    glMatrixMode(GL_MODELVIEW);

    glutMainLoop();
}

int main(int argc, char * argv[]) {
    initGL(argc, argv);
    return 0;
}

/*
glTranslated(0.0,0.0,-dist);
glRotated(-anglez,0.0,0.0,1.0);
glRotated(anglex, 1.0, 0.0,0.0);
glRotated(-angley, 0.0,1.0,0.0);
glTranslated(-VRP.x, -VRP.y, -VRP.z); */
