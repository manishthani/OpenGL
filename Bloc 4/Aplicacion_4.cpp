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

GLdouble anglex = 0.0;
GLdouble angley = 0.0;

GLdouble tx = 0.0; 
GLdouble ty = 0.0;


bool oneClick = true;
bool twoClick = false;
bool caminar = false;

GLdouble xcen, ycen, zcen,maximum;
GLdouble ymin, xmin, zmin, ymax,xmax,zmax;

Model model;
GLdouble FOV = 45.0;
GLdouble axoZoom = 0.0;

GLdouble movex = 0.1;
GLdouble movez = 0.1;
GLdouble rotatey = 10.0;


bool perspective = false;
bool invisible = false;
bool lighting = false;
bool normalVertice = true;
GLdouble speed = 0.5;

GLdouble radio = 0.0;

int altura, anchura;

int changeCorner = 1;
void setLight0(GLdouble x, GLdouble z){
      // Position and direction (spotlight)
    float posLight[] = { x, 1.5, z, 1.0 };
    float colorWhite[] = {1,1,1,0.6};
    glPushMatrix();
      glLoadIdentity();
      glLightfv(GL_LIGHT0,GL_SPECULAR,colorWhite);
      glLightfv(GL_LIGHT0,GL_DIFFUSE,colorWhite);
      glLightfv(GL_LIGHT0,GL_POSITION,posLight);
    glPopMatrix();
}

void setLight1(){
      // Position and direction (spotlight)
    float posLight1[] = { 5.0, 1.0, 5.0, 1.0 };
    float colorYellow[] = {1,1,0,0.6};
    glPushMatrix();
      glLoadIdentity();
      glLightfv(GL_LIGHT1,GL_SPECULAR,colorYellow);
      glLightfv(GL_LIGHT1,GL_DIFFUSE,colorYellow);
      glLightfv(GL_LIGHT1,GL_POSITION,posLight1);
    glPopMatrix();
}

void setLight2(){
  float posLight1[] = { -movex, 1.5, -movez, 1.0f };
    float colorWhite[] = {1,1,1,0.6};
    glPushMatrix();
      glLoadIdentity();
      glLightfv(GL_LIGHT2,GL_SPECULAR,colorWhite);
      glLightfv(GL_LIGHT2,GL_DIFFUSE,colorWhite);
      glLightfv(GL_LIGHT2,GL_POSITION,posLight1);
    glPopMatrix();
}


double max(double i, double j){
    return (i > j ) ? i : j;
}

double min(double i, double j){
    return (i > j ) ? j : i;
}

void drawSnowMan(GLdouble transNinotX , GLdouble transNinotZ){
    float ambient[4] = {1,1,1,1};
    float diffuse[4] = {1,1,1,1};
    float specular[4] = {1,1,1,0.6};
    float shininess = 120; // Mate
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    
     //ESFERA GRANDE
    glPushMatrix();
    glTranslatef(transNinotX ,0.4,transNinotZ);
    glColor4f(1.0,1.0,1.0,0.6);
    glutWireSphere(0.4,100,100);
    glPopMatrix();
  
    //ESFERA PEQUEÑA
    glPushMatrix();
    glTranslatef(transNinotX ,1.0,transNinotZ);
    glColor4f(1.0,1.0,1.0,0.6);
    glutWireSphere(0.2,100,100);
    glPopMatrix();
    //CONO 1
    glPushMatrix();
    float coneDiffuse[4] = {0.5,0.0,0.0,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, coneDiffuse);
    glTranslatef(transNinotX + 0.1 ,1.0,transNinotZ);
    glRotatef(90.0,0.0,1.0,0.0);
    glColor4f(1.0,0.0,0.0,1.0);
    glutWireCone(0.1,0.2,100,100);
    glPopMatrix();

}

void walk(){
    if(caminar){
        glLoadIdentity();
        gluLookAt(-movex,1.5,-movez, -movex+sin((rotatey - 90) *M_PI/180),1.5,-movez+cos((rotatey-90)*M_PI/180),0,1,0);
    }
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
    ycen =  ymin + (ymax - ymin) / 2 ; 
    zcen =  zmin + (zmax - zmin) / 2 ;
    maximum = max(xmax - xmin , ymax - ymin);
    maximum = max(maximum, zmax - zmin);
}

void drawPatricio(){
  glPushMatrix();
    glTranslated(-xcen,-ymin,-zcen);
    glColor3f(0,0,0);
    glBegin(GL_TRIANGLES);
        for(int i = 0; i < model.faces().size(); i++){
            const Face &f = model.faces()[i];
            if(!lighting) glColor4fv(Materials[f.mat].diffuse);
            else{
                if(!normalVertice || f.n.empty()) glNormal3dv(f.normalC);
                glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Materials[f.mat].ambient);
                glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Materials[f.mat].diffuse);
                glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Materials[f.mat].specular);
                glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Materials[f.mat].shininess);
            }
            for(int j = 0; j < 3; j++){
                if(normalVertice && !f.n.empty()){
                    const Normal &n=model.normals()[f.n[j]];
                    glNormal3dv(&n);
                }
                glVertex3dv(&model.vertices()[f.v[j]]);
            }
        }
    glEnd();
    glPopMatrix();
}

void drawGround(){
  glPushMatrix();
    float ambient[4] = {1,1,1,1};
    float diffuse[4] = {0,0,1,1};
    float specular[4] = {1,1,1,1};
    float shininess = 68;
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glBegin(GL_QUADS);
        glColor4f(0.5,0.8,0.0,0.4);
        glVertex3f(5,0.0,5);
        glVertex3f(5,0.0,-5);
        glVertex3f(-5 ,0.0,-5);
        glVertex3f(-5,0.0,5);
    glEnd();
  glPopMatrix();
}

void drawWall(GLdouble xpar, GLdouble ypar, GLdouble zpar, GLdouble x, GLdouble y, GLdouble z){
    glPushMatrix();

    float ambient[4] = {0,1,0,1};
    float diffuse[4] = {0,1,0,1};
    float specular[4] = {0,0,0,0};
    float shininess = 10; // Mate
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    
    glTranslated(x,y,z);
    glScaled(xpar, ypar, zpar);
    glutSolidCube(1);
    glPopMatrix();
}

void ini_pos_camera(){
    glLoadIdentity();
    glTranslated(0.0,0.0,-radio);
    glRotated(angley, 1.0, 0.0,0.0);
    glRotated(anglex, 0.0,1.0,0.0);
}

void refresh (void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Terreno
    drawGround();

    // Primer Patricio
    glPushMatrix();
    glTranslated(-movex,0,-movez);
    glRotatef(rotatey,0,1,0);
    glScaled((1/maximum), (1/maximum), (1/maximum));
    glRotated(-90.0,0,1,0);
    drawPatricio();
    glPopMatrix();
    setLight2();
    //Segundo Patricio
    glPushMatrix();
    glTranslated(2.5,0.0,2.5);
    glScaled((1.5/maximum), (1.5/maximum), (1.5/maximum));
    drawPatricio();
    glPopMatrix();

    //Paredes invisible o visibles
    if(!invisible){
        drawWall(10,1.5,0.2,0,0.75,-4.9);
        drawWall(0.2,1.5,4.0,1.5,0.75,2.5);
    }

    //Ninots
    drawSnowMan(2.5,-2.5);
    drawSnowMan(-2.5,2.5);
    drawSnowMan(-2.5,-2.5);

    glutSwapBuffers();
    glutPostRedisplay();
}

void setRadio(){
  
  GLdouble x1 = 5;
  GLdouble y1 = 0.0;
  GLdouble z1 = 5;
  
  GLdouble x2 = -5;
  GLdouble y2 = 1.5;
  GLdouble z2 = -5;
  
  radio = sqrt((x2 - x1)*(x2 - x1) + (y2 -y1) * (y2 -y1) + (z2 - z1) * (z2 - z1));
  radio /= 2.0;
}

void resizeWindow(int width, int height){
   anchura = width;
   altura = height;
   glViewport(0, 0, width , height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLdouble ratio = width / double(height); 
   GLdouble top;
   GLdouble left;
   GLdouble zoomTop;
   GLdouble zoomLeft; 
   if(ratio >= 1){
        top = radio * ratio;
        left = radio;
        zoomTop = axoZoom * ratio;
        zoomLeft = axoZoom;
   }
   else if(ratio < 1){
        top = radio;
        left = radio / double(ratio);
        zoomLeft = axoZoom / ratio;
        zoomTop = axoZoom;
   }
   if(!perspective) glOrtho(-top - zoomTop,top + zoomTop,-left - zoomLeft ,left + zoomLeft,-20,20);
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
    }
    else if(twoClick){
        if (y - 5 > motiony ) {
            motiony = y;
            FOV += 2;
            if(FOV > 180) FOV = 179;
            axoZoom += 0.1;
        }
        else if(y + 5 <= motiony){
            motiony = y;
            FOV-= 2;
            if(FOV <= 0) FOV = 1;
            axoZoom -= 0.1;
        }
    }
    resizeWindow(anchura,altura);
}

void mouseFunction(int key, int keyPress, int x, int y){
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
  
  if(key == 'p'){
    perspective = perspective ^ true;
    resizeWindow(anchura,altura);
  }
  if(key == 'h'){
    cout << "Tecla h: HELP" << endl;
    cout << "Tecla p :Cambiar de camara (axonometrica o perspectiva)." << endl;
    cout << "Tecla w: Avanzar Patricio" << endl;
    cout << "Tecla s: Llevar hacia atrás Patricio" << endl;
    cout << "Tecla a: Girar a Patricio hacia derecha" << endl;
    cout << "Tecla d: Girar a Patricio hacia izquierda" << endl;
    cout << "Tecla v: Hacer invisible las paredes" << endl;
    cout << "Tecla c: Camara en primera persona de Patricio" << endl;
    cout << "Tecla z: Aumentar velocidad Patricio" << endl;
    cout << "Tecla x: Disminuir velocidad Patricio" << endl;
    cout << "Tecla i: Encendido/Apagado de iluminacion" << endl;
    cout << "Tecla n: Normal por vertice/cara" << endl;
    cout << "Tecla 0: Iluminacion blanca" << endl;
    cout << "Tecla 1: Iluminacion amarilla" << endl;
    cout << "Tecla 2: Iluminacion sobre Patricio" << endl;
    cout << "Tecla m: Iluminacion blanca cambiante de esquinas del terreno" << endl;
    cout << "Tecla r: Reset de la escena" << endl;
    cout << "Tecla Escape: Salir de la aplicacion." << endl;
  }
  if(key == 'v') invisible = not invisible;
  if(key == 'w') {
    movex += sin(((rotatey + 90)*M_PI)/180) * speed;
    movez += cos(((rotatey + 90)*M_PI)/180) * speed;
    walk();
}
  if(key == 's'){
    movex -= sin(((rotatey + 90)*M_PI)/180) * speed;
    movez -= cos(((rotatey + 90)*M_PI)/180) * speed;
    walk();
  } 
  if(key == 'a') {
    rotatey += 3.0;
    walk();
  }
  if(key == 'd') {
    rotatey -= 3.0;
    walk();
  }
  if(key == 'z') speed += 0.1;
  if(key == 'x') {
    speed -= 0.1;
    if(speed <= 0.1) speed = 0.1;
  }
  if(key == 'c' and perspective) {
    caminar = true;
    walk();
  }
  if(key == 'r'){
    //Reset de camara
    glLoadIdentity();
    anglex = angley = 0.0;
    perspective = false;
    caminar = false;
    resizeWindow(anchura,altura);
    
    //Reset de iluminacion
    glDisable(GL_LIGHTING);
    lighting = false;
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glEnable(GL_LIGHT0);

  }
  if(key == 'i'){
    if(lighting) glDisable(GL_LIGHTING);
    else glEnable(GL_LIGHTING);
    lighting = not lighting;
  }
  if(key == 'n'){
    normalVertice = not normalVertice;
  }
  if(key == '0'){
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glEnable(GL_LIGHT0);

  }
  if(key == '1'){
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT2);
    glEnable(GL_LIGHT1);
  }
  if(key == '2'){
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT2);
  }
  if(key == 'm'){
    if(changeCorner == 0) setLight0(5.0,5.0);  
    else if(changeCorner == 1) setLight0(5.0,-5.0);  
    else if(changeCorner == 2) setLight0(-5.0,-5.0);
    else {
      setLight0(-5.0,5.0);
      changeCorner = -1;
    }
    ++changeCorner;
  }
  if(key == 27) exit(0);
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
   

    glEnable(GL_NORMALIZE);
    setLight1();
    setLight0(5.0,5.0);
    setLight2();
    glEnable(GL_LIGHT0); 

    model.load("Patricio.obj");
    glEnable(GL_DEPTH_TEST);
    //Define el volumen de vision que queremos ( Hace que eje z este afuera)
    setRadio();
    setCenterPoints();
    glutMainLoop();
}

int main(int argc, char * argv[]){
    initGL(argc, argv);
    return 0;
}