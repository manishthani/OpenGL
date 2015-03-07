//IDI-LAB3 -- Ernest van Sabben Alsina Grup 22
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
#include <string>
#include <limits>
#include <math.h>
#include "model.h"

using namespace std;
#define INI_X 800.0
#define INI_Y 800.0
#define INI_ROTATEX -25.0
#define INI_ROTATEY 10.0

#define ESC 27
#define SLICES 50
#define STACKS 50
#define EXTRA_DIST 1.0
#define FP_ANGLE 30.0
#define ACCELERATION 0.01
struct Vertex3d {
    Vertex x;
    Vertex y;
    Vertex z;
};

Vertex3d extrems_escena[2];
double rav, raw;
int finestra_x = INI_X;
int finestra_y = INI_Y;
double rotatex, rotatey;
int programa_estatus, character_select;
bool activate_zoom, show_walls=true;
int oldx, oldy;



struct Camera {
    double zN, zF;
    Vertex3d VRP, OBS;
    double radi, dist;
    bool perspectiva, firstPerson, tafaner;
    double zoom;
    double alfa, beta;
};
Camera camera;





class character_model {
public:

    string file;
    Model m;
    Vertex3d extrems_base[2];
    Vertex3d centre_base;
    Vertex3d extrems_cub[2];
    double scale;

    character_model(string file) {
        this->file=file;
        m.load("./models/"+file);

        double xmax,xmin,ymax,ymin,zmax,zmin;

        if(!m.vertices().empty()) {
            xmin=xmax=m.vertices()[0];
            ymin=ymax=m.vertices()[1];
            zmin=zmax=m.vertices()[2];
            for(int i=3; i<m.vertices().size(); i=i+3) {
                const double &vx=m.vertices()[i];
                const double &vy=m.vertices()[i+1];
                const double &vz=m.vertices()[i+2];
                if(vy<ymin) ymin=vy;
                if(vy>ymax) ymax=vy;
                if(vx<xmin) xmin=vx;
                if(vx>xmax) xmax=vx;
                if(vz<zmin) zmin=vz;
                if(vz>zmax) zmax=vz;
            }
            extrems_cub[0].x=xmin;
            extrems_cub[0].y=ymin;
            extrems_cub[0].z=zmin;
            extrems_cub[1].x=xmax;
            extrems_cub[1].y=ymax;
            extrems_cub[1].z=zmax;

            xmax=zmax=-numeric_limits<double>::infinity();
            xmin=zmin=numeric_limits<double>::infinity();


            for(int i=0; i<m.vertices().size(); i=i+3) {
                const double &vx=m.vertices()[i];
                const double &vy=m.vertices()[i+1];
                const double &vz=m.vertices()[i+2];

                if(vy<=ymin) {
                    if(vx<xmin) xmin=vx;
                    if(vx>xmax) xmax=vx;
                    if(vz<zmin) zmin=vz;
                    if(vz>zmax) zmax=vz;
                }
            }


        }

        extrems_base[0].x=xmin;
        extrems_base[0].y=ymin;
        extrems_base[0].z=zmin;
        extrems_base[1].x=xmax;
        extrems_base[1].y=ymax;
        extrems_base[1].z=zmax;
        centre_base.x=(xmax+xmin)/2;
        centre_base.y=ymin;
        centre_base.z=(zmax+zmin)/2;
        scale=1.0/(ymax-ymin);
    }
};

vector <character_model> cm;

class character {
public:
    double posx, posz;
    int model;
    double scale, rotate;
    double weight, anchor, height;
    double speed;
    character(int model, double x, double y, double scale) {
        posx=x;
        posz=y;
        speed=0.05;
        this->model=model;
        this->scale=scale;
        const character_model &m = cm[model];
        weight =scale*m.scale*(m.extrems_base[1].x-m.extrems_base[0].x);
        height =scale*m.scale*(m.extrems_base[1].y-m.extrems_base[0].y);
        anchor =scale*m.scale*(m.extrems_base[1].z-m.extrems_base[0].z);
    }
    bool movex(double x) {
        double posx_aux=posx+x*sin(rotate*M_PI/180);
        if(posx_aux<-5.0+weight/2) return false;
        else if(posx_aux>5.0-weight/2) return false;
        posx=posx_aux;
        return true;

    }

    bool movey(double y) {
        double posz_aux=posz+y*cos(rotate*M_PI/180);
        if(posz_aux<-5.0+weight/2) return false;
        else if(posz_aux>5.0-weight/2) return false;
        posz=posz_aux;
        return true;

    }
    void draw() {
        const character_model &cm_aux = cm[model];
        const Model &m = cm_aux.m;

        glPushMatrix();
        glTranslated(posx,0,posz);
        glRotated(rotate,0,1,0);
        glScaled(scale,scale,scale);
        glScaled(cm_aux.scale,cm_aux.scale,cm_aux.scale);
        glTranslated(-cm_aux.centre_base.x,-cm_aux.centre_base.y,-cm_aux.centre_base.z);

        glBegin(GL_TRIANGLES);
        glColor3f(0,0,0);
        for(int i=0; i<m.faces().size(); i++) {
            const Face &f=m.faces()[i];
            glColor4fv(Materials[f.mat].diffuse);
            for(int j=0; j<3; j++) {
                const Vertex &v=m.vertices()[f.v[j]];
                glVertex3dv(&v);
            }
        }
        glEnd();

        glPopMatrix();

    }
};
vector<character> c;


void draw_ninot(double x, double z) {
    glPushMatrix();
    glTranslated(x,0.4,z);
    glPushMatrix();
    glColor3f(1,1,1);
    glutSolidSphere(0.4,SLICES,STACKS);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0,0.6,0);
    glutSolidSphere(0.2,SLICES,STACKS);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0.1,0.6,0);
    glRotated(90,0,1,0);
    glColor3f(230.0/256.0,90.0/256.0,0);
    glutSolidCone(0.1,0.2,SLICES,STACKS);
    glPopMatrix();
    glPopMatrix();
}



void draw_base() {

    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(1,0,0);
      glVertex3f(-5.0,0,-5.0);;
      glVertex3f(5.0,0,-5.0);
      glVertex3f(5.0,0,5.0);
      glVertex3f(-5,0,5.0);
    glEnd();

    glPopMatrix();

}

void draw_walls() {
    glColor3f(0,1,0);

    glPushMatrix();
    glTranslatef(-4.9,0.75,0);
    glScaled(0.2,1.5,10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.5,0.75,-1.5);
    glScaled(4,1.5,0.2);
    glutSolidCube(1);
    glPopMatrix();

}
void setCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(camera.firstPerson) {
        camera.alfa=c[character_select].rotate;
        camera.OBS.x = c[character_select].posx;
        camera.OBS.y = c[character_select].height;
        camera.OBS.z = c[character_select].posz;
        camera.VRP.y= camera.OBS.y;

        if(camera.tafaner) {
            camera.VRP.x= camera.OBS.x+1*sin((camera.alfa+camera.beta)*M_PI/180);
            camera.VRP.z= camera.OBS.z+1*cos((camera.alfa+camera.beta)*M_PI/180);
        } else {
            camera.VRP.x=camera.OBS.x+1*sin(camera.alfa*M_PI/180);
            camera.VRP.z= camera.OBS.z+1*cos(camera.alfa*M_PI/180);
        }
        gluLookAt(camera.OBS.x,camera.OBS.y,camera.OBS.z,camera.VRP.x,camera.VRP.y,camera.VRP.z,0,1,0);

    } else {
        glTranslated(0,0,-camera.dist);
        glRotated(-rotatex,1,0,0);
        glRotated(-rotatey,0,1,0);
        glTranslated(-camera.VRP.x,-camera.VRP.y,-camera.VRP.z);
    }
}




void iniCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(camera.firstPerson) {
        gluPerspective(2*FP_ANGLE, rav,  0.1,  10);
    camera.radi=1;
    camera.dist=0.1;
    } else {
    //variables euler
        camera.VRP.x= (extrems_escena[1].x+extrems_escena[0].x)/2.0;
        camera.VRP.y= (extrems_escena[1].y+extrems_escena[0].y)/2.0;
        camera.VRP.z= (extrems_escena[1].z+extrems_escena[0].z)/2.0;
        camera.radi=sqrt((extrems_escena[1].x-camera.VRP.x)*(extrems_escena[1].x-camera.VRP.x)+
                         (extrems_escena[1].y-camera.VRP.y)*(extrems_escena[1].y-camera.VRP.y)+
                         (extrems_escena[1].z-camera.VRP.z)*(extrems_escena[1].z-camera.VRP.z));
        camera.dist=camera.radi+EXTRA_DIST;
        camera.zN=camera.dist-camera.radi;
        camera.zF=camera.dist+camera.radi;

        if(camera.perspectiva) {
          double alpha = atan(camera.radi/camera.dist);
          if(rav<raw) alpha = atan(tan(alpha)/rav);
          alpha= alpha*180/M_PI/camera.zoom;
          gluPerspective(2*alpha , rav,  camera.zN,  camera.zF);
        } else {
            if(rav> raw) {
                glOrtho(-camera.radi*rav/camera.zoom,camera.radi*rav/camera.zoom ,-camera.radi/camera.zoom ,camera.radi/camera.zoom ,camera.zN,camera.zF);
            } else {
                glOrtho(-camera.radi/camera.zoom ,camera.radi/camera.zoom ,-camera.radi/rav/camera.zoom ,camera.radi/rav/camera.zoom ,camera.zN,camera.zF);
            }

        }
    }
    glutPostRedisplay();
}

void refresh(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setCamera();

    draw_base();
    if(show_walls)draw_walls();
    draw_ninot(2.5,2.5);
    draw_ninot(-2.5,2.5);
    draw_ninot(-2.5,-2.5);
    for(unsigned int i=0; i<c.size(); i++)
        if(!(camera.firstPerson&&i==character_select))
            c[i].draw();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    rav = (double)w/(double)h;
    glViewport(0,0,w,h);
    iniCamera();
}

void shutdown() {
    //glutLeaveMainLoop();
}


void handleSpecialKeypress(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        if(camera.tafaner) {
            camera.beta+=c[character_select].speed*90;
            iniCamera();
        }
        break;
    case GLUT_KEY_RIGHT:
        if(camera.tafaner) {
            camera.beta-=c[character_select].speed*90;
            iniCamera();
        }
        break;
    }
}


void readKeyboard(unsigned char key, int x, int y) {
    bool moved;
    switch(key) {
    case 'h':
        cout << "LAB3-HELP:\n'r' Camera inicial\n'p' Camera Axonometric/Perspectiva\n'v' Activar/Desactivar parets\n'c' Mode rotació/ Primera persona\n'h' Mostra controls\n'wasd' Mou personatge\n't' Activar/desactivar mode tafaner\n'Left-Right' Gira la vista del tafaner\n'ESC' Surt" << endl;
        shutdown();
        break;

    case 't':
        if(camera.firstPerson) camera.tafaner= !camera.tafaner;
        iniCamera();
        break;
    case 'z':
        c[character_select].speed +=ACCELERATION;
        break;
    case 'x':
        c[character_select].speed -=ACCELERATION;
        break;
    case 'r':
        camera.firstPerson=false;
        rotatex=INI_ROTATEX;
        rotatey=INI_ROTATEY;
        camera.zoom=1.0;
    iniCamera();
        break;
    case 'p':
        camera.perspectiva=!camera.perspectiva;
        iniCamera();
        break;
    case 'v':
        show_walls = !show_walls;
        break;
    case 'c':
        camera.firstPerson = !camera.firstPerson;
        iniCamera();
        break;
    case '1':
        if(!c.empty()) character_select=0;
        break;
    case '2':
        if(c.size()>1) character_select=1;
        break;
    case 'w':
        moved=c[character_select].movex(c[character_select].speed);
        moved=c[character_select].movey(c[character_select].speed);
        break;
    case 's':
        moved=c[character_select].movex(-c[character_select].speed);
        moved=c[character_select].movey(-c[character_select].speed);
        break;
    case 'a':
        c[character_select].rotate+=c[character_select].speed*90;

        break;
    case 'd':
        c[character_select].rotate-=c[character_select].speed*90;

        break;
    case ESC:
        shutdown();
        break;

    }
    glutPostRedisplay();
}

void readMouse(int button, int state, int x, int y) {
    oldx = x;
    oldy = y;
    if(button==GLUT_RIGHT_BUTTON) activate_zoom=true;
    else activate_zoom=false;
}


void readMouseOver(int x, int y) {

    int diffx=(x-oldx);
    int diffy=(y-oldy);
    oldx = x;
    oldy = y;
    
    if(activate_zoom) {
        camera.zoom -=(double)diffy/finestra_y;
        if(camera.zoom<1) camera.zoom=1;
        iniCamera();
    } else if(!camera.firstPerson) {
            rotatey += diffx;
            rotatex += diffy;
        glutPostRedisplay();
   } 
}
void initgl() {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(INI_X,INI_Y);
    glutCreateWindow("IDI: Practica 3 OpenGL");
    glutDisplayFunc(refresh);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(readKeyboard);
    glutMouseFunc(readMouse);
    glutMotionFunc(readMouseOver);
    glutSpecialFunc(handleSpecialKeypress);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glEnable(GL_DEPTH_TEST);

    extrems_escena[0].x=-5.0;
    extrems_escena[0].y=-1.5;
    extrems_escena[0].z=-5.0;
    extrems_escena[1].x=5.0;
    extrems_escena[1].y=1.5;
    extrems_escena[1].z=5.0;
    raw= INI_X/INI_Y;
    rotatex=INI_ROTATEX;
    rotatey=INI_ROTATEY;
    camera.zoom=1;
    iniCamera();

    glutMainLoop();

}


int main(int argc, const char * argv[]) {

    cm.push_back(character_model("Patricio.obj"));
    c.push_back(character(0,0,0, 1)); // modelid, posx, posz, relative_scale
    c.push_back(character(0,2.5,-2.5, 1.5));
    c.back().rotate=90;

    
    glutInit(&argc, (char **)argv);
    initgl();
}

