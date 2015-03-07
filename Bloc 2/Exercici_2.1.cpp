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

//Variables usadas para mover la camara
int motionx = 0;
int motiony = 0;

bool xmoved = false;
bool ymoved = false;

int altura = 0;
int anchura = 0;
int posx =0;
int posy = 0;

GLdouble anglex = 0.0;
GLdouble angley = 0.0;

bool camera = true;

//Variables usadas para rotar el muneco
bool rota = false;

//Variables para escalar el muneco
bool sca = false;
bool primer = true;
int scalex = 1;
int scaley = 1;

//Variables usadas para trasladar el muneco

bool trans = false;

GLdouble scale = 1.0;

GLdouble translatex = 0.0;
GLdouble translatey = 0.0;

GLdouble rotatex = 0.0;
GLdouble rotatey = 0.0;

GLdouble anglex2 = 0.0;
GLdouble angley2 = 0.0;

GLdouble tx = 0.0;
GLdouble ty = 0.0;

int resolx = 600;
int resoly = 600;

void drawLines(){
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

void drawSnowMan(){
    
    //CONO
    glPushMatrix();
    //------------------------------------------
    glTranslatef(tx,ty,0.0);
    glScalef(scale,scale,scale);
    //------------------------------------------
    glRotatef(angley2,1.0,0.0,0.0);
    glRotatef(anglex2,0.0,1.0,0.0);
    glTranslatef(0.1,0.6,0.0);
    glRotatef(90.0,0.0,1.0,0.0);

    glColor4f(1.0,0.0,0.0,1.0);
    glutWireCone(0.1,0.2,50,50);
    glPopMatrix();

    //ESFERA GRANDE
    glPushMatrix();
    //------------------------------------------
    glTranslatef(tx,ty,0.0);
    glScalef(scale,scale,scale);
    /*glTranslatef(0.1,0.6,0.0);*/
    glRotatef(angley2,1.0,0.0,0.0);
    glRotatef(anglex2,0.0,1.0,0.0);
    //------------------------------------------
    glColor4f(1.0,1.0,1.0,0.6);
    glutWireSphere(0.4,50,50);
    
    glPopMatrix();
  
    //ESFERA PEQUEÑA
    glPushMatrix();

    glTranslatef(tx,ty,0.0);
    glScalef(scale,scale,scale);
    glRotatef(angley2,1.0,0.0,0.0);
    glRotatef(anglex2,0.0,1.0,0.0);
    glTranslatef(0.0,0.6,0.0);
   
    glColor4f(1.0,1.0,1.0,0.6);
    glutWireSphere(0.2,50,50);
    glPopMatrix();

}

void refresh (void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawSnowMan();
    drawLines();
    glLoadIdentity();
    glRotated(angley, 1.0f,0.0f,0.0f);
    glRotated(anglex, 0.0f,1.0f,0.0f);
    glutSwapBuffers();
    glutPostRedisplay();
}

void resizeWindow(int width, int height){
    resolx = width;
    resoly = height;
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
    else if(sca){
        if(primer) {
            scalex = x;
            scaley = y;
            primer = false;
        }
        else{
            if(x <= scalex) {
                if(y >= scaley)scale -= 0.05;
                else scale += 0.05;
            }
            else {
                if( y <= scaley) scale += 0.05;
                else scale -= 0.05;
            }
            if(scale < 0.05) scale = 0.05;
            scalex = x;
            scaley = y;
        }
    }
    else if(rota){
        if (x - 5 > rotatex) {
            anglex2 += 4.0;
            rotatex = x;
            if(anglex2 >= 360) anglex2 = 0.0;
        }
        else if(x + 5 <= rotatex ){
            rotatex = x;
            anglex2 -=4.0;
            if(anglex2 <= -360) anglex2 = 0.0;

        }
        if (y - 5 > rotatey ) {
            angley2 +=4.0;
            rotatey= y;
            if(angley2 >= 360) angley2 = 0.0;

        }
        else if(y + 5 <= rotatey ){
            rotatey = y;
            angley2 -=4.0;
            if(angley2 <= -360) angley2 = 0.0;
        }
    }
    else if(trans){
         if (x - 5 > translatex) {
            translatex = x;
            tx += 0.05;
            
        }
        else if(x + 5 <= translatex ){
            translatex = x;
            tx -= 0.05;
        }
        if (y - 5 > translatey ) {
            ty -= 0.05;
            translatey = y;
        }
        else if(y + 5 <= translatey ){
            translatey = y;
            ty += 0.05;
        }
    }
}

void mouseFunction(int key, int keyPress, int x, int y){
    //Para poner los bools a false de motionFunc, usar KeyPres
}

void keyboardFunc(unsigned char key, int x, int y){
  if(key == 's'){
      sca = true;
      camera = false;
  }
  else if(key == 'r'){
    rota = true;
    camera = false;
  }
  else if(key == 't'){
    trans = true;
    camera = false;
  }
  else if(key == 'c'){
    trans = rota = sca = false;
    camera = true;
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
    //Para definir la opacidad de los objetos 3d en Opengl
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
