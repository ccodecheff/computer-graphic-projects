
#include <string.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>


#define PI 3.14159

using namespace std;

bool WireFrame= false;  //to toggle Wireframe
bool bunny = false;  //to display bunny
bool ateneam = false;  //to display ateneam
int factor;  //factor for model

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

typedef struct{
float x;  // x coordinate
float y;  // y coordinate
float z;  // z coordinate
}vec3;

typedef struct {
    int v1;
    int t1;
    int n1;
    int v2;
    int t2;
    int n2;
    int v3;
    int t3;
    int n3;
} faces;

float RotateX =0;
float RotateY =0;
float zoom =-50;

float Wwidth,Wheight;

vec3* vertex;
vec3* normal;
vec3* texture;
faces* face;

vec3* mem;
faces* fmem;

    int vNum=0;
    int nNum=0;
    int tNum=0;
    int fNum=0;

void loadObj(char *fname)
{
    char line[1500];
    const char* ch;
    //int factor=175;
    float tmp[4];


    FILE *file;
    file = fopen(fname,"r");
    if(!file)
    {
      cerr << "Error: can't open file " << fname << endl;
      exit(1);
    }
    else{

    vertex =(vec3*)malloc(sizeof(vec3)); //allocate initial memory for a vertex
    normal=(vec3*)malloc(sizeof(vec3)); //allocate initial memory for normal
    texture=(vec3*)malloc(sizeof(vec3)); //allocate initial memory for texture
    face =(faces*)malloc(sizeof(faces)); //allocate initial memory for faces indices

    mem=(vec3*)malloc(sizeof(vec3));
    fmem =(faces*)malloc(sizeof(faces));

          while( fgets(line, sizeof(line), file) != NULL )
         {

            int i =0;
            ch=NULL;
            ch = strtok (line," ");

            //read vertices
             if(strcmp(ch,"v")==0)
            {

               for(int i=0;i<4;i++){

                    tmp[i]=atof(ch);
                      ch = strtok (NULL, " \n");

                }

                vertex[vNum].x= tmp[1]/factor;
                vertex[vNum].y= tmp[2]/factor;
                vertex[vNum].z= tmp[3]/factor;
                normal[vNum].x= 0.0;
                normal[vNum].y= 0.0;
                normal[vNum].z= 0.0;
                vNum++;

               mem =(vec3*)realloc(vertex,(vNum+1)*sizeof(vec3)); //reallocate memory for vertex

               if(mem!=NULL){
                 vertex=mem;
               }
               mem =(vec3*)realloc(normal,(vNum+1)*sizeof(vec3)); //reallocate memory for normal

               if(mem!=NULL){
                 normal=mem;
               }
            }
            else if(strcmp(ch,"vn")==0) //read normals
            {

              for(int i=0;i<4;i++)
                {
                    tmp[i]=atof(ch);
                      ch = strtok (NULL, " \n");
                }

                normal[nNum].x= tmp[1];
                normal[nNum].y= tmp[2];
                normal[nNum].z= tmp[3];
                nNum++;

                 mem =(vec3*)realloc(normal,(nNum+1)*sizeof(vec3)); //reallocate memory for normal

               if(mem!=NULL){
                 normal=mem;
               }
            }
            else if(strcmp(ch,"vt")==0) //read textures
            {
               for(int i=0;i<4;i++)
                {
                    tmp[i]=atof(ch);
                      ch = strtok (NULL, " \n");
                }

                texture[tNum].x= tmp[1];
                texture[tNum].y= tmp[2];
                texture[tNum].z= tmp[3];
                tNum++;

                 mem =(vec3*)realloc(texture,(tNum+1)*sizeof(vec3)); //reallocate memory for texture

               if(mem!=NULL){
                 texture=mem;
               }
            }
             else if(strcmp(ch,"f")==0) //read faces - v index/ vt index / vn index
             {

            int ind[12];
              for(int i=0;i<11;i++)
            {
                ind[i]=atoi(ch);
                ch = strtok (NULL, " /");
            }


            face[fNum].v1 = ind[1] - 1;
            face[fNum].v2 = ind[4] - 1;
            face[fNum].v3 = ind[7] - 1;


            face[fNum].t1 = ind[2] - 1;
            face[fNum].t2 = ind[5] - 1;
            face[fNum].t3 = ind[8] - 1;

            face[fNum].n1 = ind[3] - 1;
            face[fNum].n2 = ind[6] - 1;
            face[fNum].n3 = ind[9] - 1;

            fNum++;

              fmem =(faces*)realloc(face,(fNum+1)*sizeof(faces)); //reallocate memory for faces array

               if(fmem!=NULL){
                 face=fmem;
               }

	}
         }
   }fclose(file);
 cout<<"vcount : "<<vNum<<" ncount : "<<nNum<<" count : "<<tNum<<" fcount : "<<fNum<<endl;
}

void calculateNormal()
{
     for(int i=0;i<fNum;i++){

    vec3 vertex1 = vertex[face[i].v1];
    vec3 vertex2 = vertex[face[i].v2];
    vec3 vertex3 = vertex[face[i].v3];

    float Vx = vertex2.x - vertex1.x;
    float Vy = vertex2.y - vertex1.y;
    float Vz = vertex2.z - vertex1.z;

    float Ux = vertex1.x - vertex3.x;
    float Uy = vertex1.y - vertex3.y;
    float Uz = vertex1.z - vertex3.z;

    //cross product
    float Nx = (Uy * Vz) - (Uz * Vy);
    float Ny = (Uz * Vx) - (Ux * Vz);
    float Nz = (Ux * Vy) - (Uy * Vx);

    float length = sqrt( Nx * Nx + Ny * Ny + Nz * Nz);

    float *normalNew = new float[3];

      normalNew[0] = Nx/ length; //normalNew X value
      normalNew[1] = Ny/length; // normalNew Y Value
      normalNew[2] = Nz/ length; // normalNew Z Value


        normal[face[i].v1].x = (normal[face[i].v1].x + normalNew[0])/2;
        normal[face[i].v1].y = (normal[face[i].v1].y + normalNew[1])/2;
        normal[face[i].v1].z = (normal[face[i].v1].z + normalNew[2])/2;

        normal[face[i].v2].x = (normal[face[i].v2].x + normalNew[0])/2;
        normal[face[i].v2].y = (normal[face[i].v2].y + normalNew[1])/2;
        normal[face[i].v2].z = (normal[face[i].v2].z + normalNew[2])/2;

        normal[face[i].v3].x = (normal[face[i].v3].x + normalNew[0])/2;
        normal[face[i].v3].y = (normal[face[i].v3].y + normalNew[1])/2;
        normal[face[i].v3].z = (normal[face[i].v3].z + normalNew[2])/2;

    }
}

static void resize(int width, int height)
{
     double Ratio;

     Wwidth = (float)width;
     Wheight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective (45.0f,Ratio,0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }

 static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 100.0);

     if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME


    glTranslated(0.0,0.0,zoom);
    glRotated(RotateX,1,0,0);
    glRotated(RotateY,0,1,0);

    glPushMatrix();
       for(int i=0;i<fNum;i++)
		{

        glBegin(GL_TRIANGLES);

        glNormal3f(normal[face[i].n1].x,normal[face[i].n1].y,normal[face[i].n1].z);
        glVertex3f(vertex[face[i].v1].x,vertex[face[i].v1].y,vertex[face[i].v1].z);

        glNormal3f(normal[face[i].n2].x,normal[face[i].n2].y,normal[face[i].n2].z);
        glVertex3f(vertex[face[i].v2].x,vertex[face[i].v2].y,vertex[face[i].v2].z);

        glNormal3f(normal[face[i].n3].x,normal[face[i].n3].y,normal[face[i].n3].z);
        glVertex3f(vertex[face[i].v3].x,vertex[face[i].v3].y,vertex[face[i].v3].z);

            glEnd();

		}
    glPopMatrix();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case 'w':
            WireFrame =!WireFrame;
            break;

        case 'b':
            bunny =!bunny;
            if(bunny==true)
            {
                ateneam=false;
                vNum=0;
                nNum=0;
                tNum=0;
                fNum=0;
                factor=20;
                loadObj("bunny.obj");
                calculateNormal();
            }
            break;

        case 'a':
            ateneam =!ateneam;
            if(ateneam==true)
            {
                bunny=false;
                vNum=0;
                nNum=0;
                tNum=0;
                fNum=0;
                factor=200;
                loadObj("ateneam.obj");
            }
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
     switch(key)
    {
    case GLUT_KEY_END:
	  zoom += (float) 1.0f;
		break;
	case GLUT_KEY_HOME:
	  zoom -= (float) 1.0f;
		break;
    case GLUT_KEY_UP:
	 	RotateX = ((int)RotateX +2)%360;
		break;
	case GLUT_KEY_DOWN:
		RotateX = ((int)RotateX -2)%360;
		  break;
	case GLUT_KEY_LEFT:
    	RotateY =((int)RotateY +2)%360;
		break;
	case GLUT_KEY_RIGHT:
		RotateY = ((int)RotateY -2)%360;
		 break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // assign a color you like

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1920,1080);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 5 - WaveFront obj Model Loader - Press 'a' for ateneam or 'b' for bunny");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);


    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
