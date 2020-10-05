// csci272 project 3
// karan rathore

#include <SOIL.h>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
#define PI 3.14159
using namespace std;
#define PI 3.14159
GLfloat matrixX[16]; // matrix initialization
GLfloat matrixY[16];
GLfloat matrixZ[16];
GLfloat x, y, z, w;

//Texture Setup
GLuint tex[4];


int width,height; // width & height for the image reader
unsigned char* image;
GLUquadricObj*sphere= NULL;			//	global	variable
GLfloat angle1, angle2, angle3, sunangle =0;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


bool start_flag= false; // boolean for special key

/* GLUT callback Handlers */

void CreateFromAxisAngle(GLfloat X, GLfloat Y, GLfloat Z, GLfloat degree)
{
 /* First we want to convert the degrees to radians since the angle is assumed to be in radians*/
GLfloat angle = (GLfloat)((degree / 180.0f) * PI);
 /* Here we calculate the sin( theta / 2) once for optimization */
GLfloat result = (GLfloat)sin( angle / 2.0f );
 /* Calcualte the w value by cos( theta / 2 ) */
w = (GLfloat)cos( angle / 2.0f );
 /* Calculate the x, y and z of the quaternion */
x = (GLfloat)(X * result);
y = (GLfloat)(Y * result);
z = (GLfloat)(Z * result);
}

void CreateMatrix(GLfloat *pMatrix)
{
// First row
pMatrix[ 0] = 1.0f - 2.0f * ( y * y + z * z );
pMatrix[ 1] = 2.0f * (x * y + z * w);
pMatrix[ 2] = 2.0f * (x * z - y * w);
pMatrix[ 3] = 0.0f;
// Second row
pMatrix[ 4] = 2.0f * ( x * y - z * w );
pMatrix[ 5] = 1.0f - 2.0f * ( x * x + z * z );
pMatrix[ 6] = 2.0f * (z * y + x * w );
pMatrix[ 7] = 0.0f;

//Third Matrix

pMatrix[ 8] = 2.0f * ( x * z + y * w );
pMatrix[ 9] = 2.0f* ( z * y - x * w );
pMatrix[ 10] = 1.0f - 2.0f *(x * x + y * y );
pMatrix[ 11] = 0.0f;

pMatrix[ 12] = 0.0f;
pMatrix[13] = 0.0f;
pMatrix[14] = 0.0f;
pMatrix[15] = 1.0f;


}
static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{

     bool WireFrame= false;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,-10,10,0.0,0.0,0.0,0.0,-1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
        glScalef(0.2,0.2,0.2);
    glPushMatrix();// sun sphere
        glBindTexture(GL_TEXTURE_2D,tex[0]); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
        glTranslatef(0,0.0,0.0); // positioning
        gluSphere(sphere,6.0,50,50);
    glPopMatrix();

glPushMatrix(); // planet B
        glBindTexture(GL_TEXTURE_2D,tex[1]); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
        CreateMatrix(matrixX); /* initial quatonion */
        CreateFromAxisAngle(0,1,0,angle3); /* quatonion for y rotation */
        glMultMatrixf(matrixX); // final matrix for Rotation
        glTranslatef(26.3,0.0,0.0);
        gluSphere(sphere,2.6,50,50);

    glPopMatrix();//

      glPushMatrix(); //Planet A (earth)
        glBindTexture(GL_TEXTURE_2D,tex[2]); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
        CreateMatrix(matrixX); /* initial quatonion */
        CreateFromAxisAngle(0,1,0,angle1); /* quatonion for y rotation */
        glMultMatrixf(matrixX);
        glTranslatef(-16.5,0.0,0.0);
        gluSphere(sphere,3.0,50,50);

      glPushMatrix();// moon
           glBindTexture(GL_TEXTURE_2D,tex[3]); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
           CreateMatrix(matrixX); /* initial quatonion */
           CreateFromAxisAngle(0,1,0,angle2); /* quatonion for y rotation */
           glMultMatrixf(matrixX);
           glTranslatef(-4.7,0.0,0.0);
           gluSphere(sphere,1.8,50,50);
      glPopMatrix();

    glPopMatrix();

   glFlush();
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
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP: // when user press UP key planets will start rotating
    start_flag=true;
    break;
    case GLUT_KEY_DOWN: //when user will press DOWN key planets will stop rotating
    start_flag=false;
    break;


   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
//If statement for angle Change

  if (start_flag==true)
    {
        angle1+=0.05;
        angle2+=0.03;
        angle3-=0.02;
        sunangle-=0.006;
    }
}
void T_load(char* Filepath, GLuint &tex)
 {
    // int width, height;
    //unsigned char *image;
    glBindTexture(GL_TEXTURE_2D,tex);
     image = SOIL_load_image(Filepath, &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
static void init(void)

{     glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
        glEnable(GL_TEXTURE_2D);
        glGenTextures(4, tex);
        T_load("img/sun1.jpg",tex[0] );// image of sun
        T_load("img/a.jpg",tex[1] ); // image of planet b
        T_load("img/earth1.jpg",tex[2]); // image of earth
        T_load("img/moon.jpg",tex[3]); // image of moon


sphere = gluNewQuadric();
gluQuadricDrawStyle(sphere, GLU_FILL);
gluQuadricTexture(sphere, GL_TRUE);
gluQuadricNormals(sphere, GLU_SMOOTH);

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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

}
/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);// It has the main code
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys); // For special keys like UP|| DOWN
    glutIdleFunc(idle);//For idle func()
    glutMainLoop();
    return EXIT_SUCCESS;
}
