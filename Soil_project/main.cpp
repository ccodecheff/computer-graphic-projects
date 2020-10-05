#include <SOIL.h>
#include <math.h>
using namespace std;
bool WireFrame= false;
GLuint tex; // Texture pointer
glGenTextures(1, &tex); // Possible to have 1D, 2D and even 3D textures
GLdouble TranslateX,TranslateY,Zoom,RotateX,RotateY,RotateZ;
glPushMatrix();
 glTranslated(TranslateX,TranslateY,Zoom);
 glRotated(RotateX,1,0,0);
 glRotated(RotateY,0,1,0);
 glRotated(RotateZ,0,0,1);
 glBegin(GL_QUADS);
 glNormal3f(0.0f, 0.0f, 1.0f); // Normal
 glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
 glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
 glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
 glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
 glNormal3f(0.0f, 0.0f, -1.0f); // Normal Back Face
 glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
 glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
 glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
 glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
 glNormal3f(0.0f, 1.0f, 0.0f); // Normals Top Face
 glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
 glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
 glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
 glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
 glNormal3f(0.0f, -1.0f, 0.0f); // Bottom Face
 glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
 glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
 glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
 glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
 glVertex3f(1.0f, 0.0f, 0.0f); // Right face
 glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
 glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
 glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
 glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
 glNormal3f(-1.0f, 0.0f, 0.0f); // Left Face
 glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
 glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
 glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
 glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
 glEnd();
 glPopMatrix();


 void Specialkeys(int key, int x, int y)
{
 switch(key)
 {
 case GLUT_KEY_END:
 Zoom += (float) 1.0f; // Zoom in
break;
case GLUT_KEY_HOME:
 Zoom -= (float) 1.0f; // Zoom Out
break;
 case GLUT_KEY_UP: //Clockwise rotation over X
RotateX = ((int)RotateX +2)%360;
break;
case GLUT_KEY_DOWN: //Counter Clockwise rotation over X
RotateX = ((int)RotateX -2)%360;
 break;
case GLUT_KEY_LEFT: // Clockwise rotation over Y
 RotateY =((int)RotateY +2)%360;
break;
case GLUT_KEY_RIGHT:
RotateY = ((int)RotateY -2)%360; //Counter Clockwise rotation over Y
break;
 }
glutPostRedisplay();
}

static void init(void)
{
 glEnable(GL_TEXTURE_2D);
 glGenTextures(1, &tex);
 glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
 glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
 int width, height; // width & height for the image reader
 unsigned char* image;

 image = SOIL_load_image("img/1.jpeg", &width, &height, 0, SOIL_LOAD_RGB);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
// binding image data
 SOIL_free_image_data(image);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}
