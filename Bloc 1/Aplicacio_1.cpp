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


bool motion = true;
bool savePoint = false;
int cont = 0;
int altura = 0;
int anchura = 0;
int posx =0;
int posy = 0;

GLdouble vx1 = -1/double(2);
GLdouble vy1 = -1/double(3);

GLdouble vx2 = 1/double(2);
GLdouble vy2 = -1/double(3);

GLdouble vx3 = 0.0f;
GLdouble vy3 = 2/double(3);

vector<pair<GLdouble,GLdouble> > vertexs;

void drawTriangle(){
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(vx1, vy1, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vx2 , vy2, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vx3, vy3, 0.0f);

    glEnd();
}

void refresh (void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(40,1.0,0.0,0.0);
    drawTriangle();
    glutSwapBuffers();
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
    //Establece el nuevo color que usara para limpiar el buffer al hacer glClear
    cout << y /double(altura) << endl;
    if(motion)glClearColor(y /double(altura)+ 0.2, y /double(altura)+0.2,y /double(altura),0.0);
    //Se utiliza esto para evitar refresh() cada vez que lo pide una funcion, con esto se hace una vez (mas eficiente).
    glutPostRedisplay();
}

void mouseFunction(int key, int keyPress, int x, int y){
    if(savePoint){
        if(!keyPress){
            GLdouble mousex = (x - posx - (anchura/2) ) / double(anchura/2);
            GLdouble mousey = -1*((y - posy - (altura/2) ) / double(altura/2));

            pair<GLdouble,GLdouble> pos;
            pos.first = mousex;
            pos.second = mousey;
            vertexs[cont] = pos;
            ++cont;
            if(cont >= 3){
                vx1 = vertexs[0].first;
                vy1 = vertexs[0].second;

                vx2 = vertexs[1].first;
                vy2 = vertexs[1].second;

                vx3 = vertexs[2].first;
                vy3 = vertexs[2].second;
                cont = 0;
                savePoint = false;
            }
        }
    }
    glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y){
    if(key == 'h') {
        cout << "Panel de Ayuda." << endl;
        cout << "Presiona la letra t y selecciona 3 puntos para dibujar el nuevo triangulo" << endl;
        cout << "Presiona el raton y arrastralo para cambiar el color de fondo" << endl;
        cout << "Presiona la letra f para activar o desactivar la funcionalidad de cambiar el color de fondo" << endl;
        cout << "Presiona la tecla ESC para salir de la aplicacion" << endl;
    }
    else if(key == 'f') motion = motion ^ 1;
    else if(key == 't') {
        savePoint = true;
        vertexs = vector<pair<GLdouble,GLdouble> >(3);
    }
    else if(key == 27)  exit(0);
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
    glutMainLoop();
}

int main(int argc, char * argv[]) {
    initGL(argc, argv);
    return 0;
}
