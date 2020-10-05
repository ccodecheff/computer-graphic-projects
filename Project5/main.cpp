#include <SOIL.h>
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
bool Flag1= false;  //to display bunny
bool Flag2 = false;  //to display ateneam
int factor;  //factor for model resize

GLuint tex[2]; // pointer variable for texture
int width,height; // width & height for the image reader
unsigned char* image; // image for texture

//rotate variable and zoom variable
float RotateX =0;
float RotateY =0;
float zoom =-50;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

// defining structure for vector3 vertices
typedef struct{
float x;  // x coordinate
float y;  // y coordinate
float z;  // z coordinate
}vector3;
 // structure for face - vertex, Vn,texture

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

    int vertex_num=0; //variable for vector data
    int normal_num=0;  //variable for Vn vector data
    int texture_num=0; //variable for texture vector data
    int face_num=0; //variable for face data


float Wwidth,Wheight;
// assign vector vertex,Vn and texture
// v is a vertex
//vt is the texture coordinate of one vertex
//vn is the Vn of one vertex
//face is a face
vector3* vertex;
vector3* Vn;
vector3* texture;
// assigning face vertex
faces* face;

vector3* vector_Mem; //struct variable for vector member
faces* fmem; // struct variable for face member



void Calculate_N() // calculate Vn for object file
{


    //for till face vector
     for(int i=0;i<face_num;i++){

    vector3 vertex1 = vertex[face[i].v1];
    vector3 vertex2 = vertex[face[i].v2];
    vector3 vertex3 = vertex[face[i].v3];
// values for U vector
    float Vx = vertex2.x - vertex1.x;
    float Vy = vertex2.y - vertex1.y;
    float Vz = vertex2.z - vertex1.z;
// values for U vector
    float Ux = vertex1.x - vertex3.x;
    float Uy = vertex1.y - vertex3.y;
    float Uz = vertex1.z - vertex3.z;

    //cross product to calculate Vn
    float Nx = (Uy * Vz) - (Uz * Vy);
    float Ny = (Uz * Vx) - (Ux * Vz);
    float Nz = (Ux * Vy) - (Uy * Vx);
// calculate length of vector
    float length = sqrt( Nx * Nx + Ny * Ny + Nz * Nz);

    float *Nv = new float[3]; // Nv variable to store vector Vnization
//normalization for vector Nvx,Nvy, Nvz
      Nv[0] = Nx/ length; //Nv X value
      Nv[1] = Ny/length; // Nv Y Value
      Nv[2] = Nz/ length; // Nv Z Value

// assigning the calculated normal values to specified x,y,z vector of face[i].vertex index

        // updation of calculated  length in face index face[i].v1 of normal vector Nv[0]
        Vn[face[i].v1].x = (Vn[face[i].v1].x + Nv[0])/2;
		// updation of calculated  length in face index face[i].v1 of normal vector Nv[1]
        Vn[face[i].v1].y = (Vn[face[i].v1].y + Nv[1])/2;
		// updation of calculated  length in face index face[i].v1 of normal vector Nv[2]
        Vn[face[i].v1].z = (Vn[face[i].v1].z + Nv[2])/2;

        Vn[face[i].v2].x = (Vn[face[i].v2].x + Nv[0])/2;   // same for vertex v2
        Vn[face[i].v2].y = (Vn[face[i].v2].y + Nv[1])/2;  // same for vertex v2
        Vn[face[i].v2].z = (Vn[face[i].v2].z + Nv[2])/2;  // same for vertex v2

        Vn[face[i].v3].x = (Vn[face[i].v3].x + Nv[0])/2; // same for vertex v3
        Vn[face[i].v3].y = (Vn[face[i].v3].y + Nv[1])/2;  // same for vertex v3
        Vn[face[i].v3].z = (Vn[face[i].v3].z + Nv[2])/2; // same for vertex v3

    }
}
// resize function
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
void load_Object(char *fname)
{
    char line[2500];
    const char* ch; // variable to read each character
    float tmp[4];


    FILE *file; // File function
    file = fopen(fname,"r"); // opening an object file
    if(!file) // no file found
    {
      cerr << "Error: can't open file " << fname << endl;
      exit(1);
    }
    else{

    vertex =(vector3*)malloc(sizeof(vector3)); //allocate initial memory for a vertex
    cerr << "Sucessfully allocated memory for vertex " <<" "<<vertex<< endl;

    Vn=(vector3*)malloc(sizeof(vector3)); //allocate initial memory for Vn
     cerr << "Sucessfully allocated memory for Vn" <<" "<<Vn<< endl;

    texture=(vector3*)malloc(sizeof(vector3)); //allocate initial memory for texture

    face =(faces*)malloc(sizeof(faces)); //allocate initial memory for faces indices

	 cerr << "Sucessfully allocated memory for face " <<" "<<face<< endl;


    vector_Mem =(vector3*)malloc(sizeof(vector3)); //allocate initial memory for mem
    fmem =(faces*)malloc(sizeof(faces));  //allocate initial memory for face

          while( fgets(line, sizeof(line), file) != NULL ) // reading each line of object file
         {

            int i =0;
            ch=NULL;
            ch = strtok (line," ");

            //read vertices values  "v -1.000000 -1.000000 1.000000" and storing it in temp varibale
             if(strcmp(ch,"v")==0) // if any vertices starts with v
            {

               for(int i=0;i<4;i++) {
                    tmp[i]=atof(ch); // assign to temp variable
                      ch = strtok (NULL, " \n"); // splittingch with null delimiter
                }


              //  initial value for vertices values
                vertex[vertex_num].x= tmp[1]/factor;
                vertex[vertex_num].y= tmp[2]/factor;
                vertex[vertex_num].z= tmp[3]/factor;

             // initial value for normal
			   Vn[vertex_num].x= 0.0;
                Vn[vertex_num].y= 0.0;
                Vn[vertex_num].z= 0.0;
                vertex_num++;
			// reallocating vector_mem value
              vector_Mem =(vector3*)realloc(vertex,(vertex_num+1)*sizeof(vector3)); //reallocate memory for vertex

               if(vector_Mem!=NULL){
                 vertex=vector_Mem; // allocating new memory of vertex
               }
              vector_Mem =(vector3*)realloc(Vn,(vertex_num+1)*sizeof(vector3)); //reallocate memory for Vn

               if(vector_Mem!=NULL){
                 Vn=vector_Mem; // allocating new memory of normal
               }
            }
            else if(strcmp(ch,"vn")==0) //read normal values "vn -1.0000 0.0000 0.0000 "from oject file which has normal
            {

              for(int i=0;i<4;i++)
                {
                    tmp[i]=atof(ch); // assign to temp variable
                      ch = strtok (NULL, " \n"); // splittingch with null delimiter
                }

			// assigning new values to normal vector from temp variable
                Vn[normal_num].x= tmp[1];
                Vn[normal_num].y= tmp[2];
                Vn[normal_num].z= tmp[3];
                normal_num++; // incrementor till end of file for normal vector

                 vector_Mem =(vector3*)realloc(Vn,(normal_num+1)*sizeof(vector3)); //reallocate memory for Vn

               if(vector_Mem!=NULL){
                 Vn=vector_Mem; // allocate normal vector memory
               }
            }
            else if(strcmp(ch,"vt")==0) //read textures
            {
               for(int i=0;i<4;i++)
                {
                    tmp[i]=atof(ch); // assign to temp variable
                      ch = strtok (NULL, " \n"); // splitting ch with null delimiter
                }

                texture[texture_num].x= tmp[1]; // assigning texture value
                texture[texture_num].y= tmp[2];
                texture[texture_num].z= tmp[3];
                texture_num++;

                 vector_Mem =(vector3*)realloc(texture,(texture_num+1)*sizeof(vector3)); //reallocate memory for texture

               if(vector_Mem!=NULL){
                 texture=vector_Mem; // assign new texture memory
               }
            }
             else if(strcmp(ch,"f")==0) //read faces
             {

            int temp[12];
              for(int i=0;i<11;i++)
            {
                temp[i]=atoi(ch);
                ch = strtok (NULL, " /");// splitting ch with "/" delimiter
            }

// assigning face vale to the face vertices  from f 1//1 2//1 4//1 3//1 this format
// f → v index /vt index /vn index
            face[face_num].v1 = temp[1] - 1;
            face[face_num].v2 = temp[4] - 1;
            face[face_num].v3 = temp[7] - 1;


            face[face_num].t1 = temp[2] - 1;
            face[face_num].t2 = temp[5] - 1;
            face[face_num].t3 = temp[8] - 1;

            face[face_num].n1 = temp[3] - 1;
            face[face_num].n2 = temp[6] - 1;
            face[face_num].n3 = temp[9] - 1;

            face_num++; // counter to read all face vertices

              fmem =(faces*)realloc(face,(face_num+1)*sizeof(faces)); //reallocate memory for faces array

               if(fmem!=NULL){
                 face=fmem; // reassigning face memory
               }

	}
         }
   }fclose(file); // end of file
 cout<<"vertiex : "<<vertex_num<<" normal : "<<normal_num<<" texture: "<<texture_num<<" face : "<<face_num<<endl;
 cout<<"press 1 for atneam"<<"\n"<<"press 2 for rabit";

}


 static void display(void) // display function
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 100.0); // gl lookat function

     if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME


    glTranslated(0.0,0.0,zoom); // for zoom in and out
    glRotated(RotateX,1,0,0); // for model roataion in x direction
    glRotated(RotateY,0,1,0); // for model roataion in x direction

    glPushMatrix(); // for model load

       for(int i=0;i<face_num;i++)
		{

        glBegin(GL_TRIANGLES); // triangle function
        glNormal3f(Vn[face[i].n1].x,Vn[face[i].n1].y,Vn[face[i].n1].z);
        glTexCoord2f(texture[face[i].t1].x,texture[face[i].t1].y);
        glVertex3f(vertex[face[i].v1].x,vertex[face[i].v1].y,vertex[face[i].v1].z);

       glNormal3f(Vn[face[i].n2].x,Vn[face[i].n2].y,Vn[face[i].n2].z);
        glTexCoord2f(texture[face[i].t2].x,texture[face[i].t2].y);
        glVertex3f(vertex[face[i].v2].x,vertex[face[i].v2].y,vertex[face[i].v2].z);

        glNormal3f(Vn[face[i].n3].x,Vn[face[i].n3].y,Vn[face[i].n3].z);
         glTexCoord2f(texture[face[i].t3].x,texture[face[i].t3].y);
        glVertex3f(vertex[face[i].v3].x,vertex[face[i].v3].y,vertex[face[i].v3].z);

            glEnd();

		}
    glPopMatrix();
    glutSwapBuffers();
}
// Note: Any assigned keys (1, 2, 3 , q ) can be free at any point of time - no need to close the screen and restart
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q': // press q key to exit from current screen
            exit(0);
            break;

        case 'w':
            WireFrame =!WireFrame; // to check wireframe of model
            break;

        case '1': // press 1 to display bunny

           Flag1 = true; // set flag1 to true for bunny
            if(Flag1==true)
            {
                Flag2=false; // set flag2 to false if bunny is in display

// reinitilizating coordinate data the `the value to 0
                vertex_num=0;
                normal_num=0;
                texture_num=0;
                face_num=0;
                factor=30;
                glBindTexture(GL_TEXTURE_2D,tex[0]);// loading texture for bunny
                load_Object("bunny.obj");// loading bunny object file to loadobject function
                Calculate_N();// calculating normal for bunny
            }
            break;

        case '2':

          Flag2 =!false; // updating flag value for anteneam
            if(Flag2==true)
            {
                Flag1=false;
                vertex_num=0;
                normal_num=0;
                texture_num=0;
                face_num=0;
                factor=200; // setting up factor to 200
               glBindTexture(GL_TEXTURE_2D,tex[1]);
                 load_Object("ateneam.obj"); // loading anteneam file with normals
            }
            break;
             case '3':
                  free(vertex); // deallocating memory for vertex
                  free(Vn); // deallocating memory for normal
                  free(face); // deallocating memory for face vector
                  free(texture); // deallocating memory for texture
                  vertex_num=0;
                  normal_num=0;
                  texture_num=0;
                  face_num=0;
                  cout<<"   "<<"\n";
                  cout<<"   "<<"\n";
                  cout<<"   "<<"\n";
                 cout<<"  vertex, Vn, face and texture vectors memory is deallocated";

               break;

    }
}
// special keys function for rotation and zoom
void Specialkeys(int key, int x, int y)
{
     switch(key)
    {
    case GLUT_KEY_END:
	  zoom += (float) 1.0f; // zoom in
		break;
	case GLUT_KEY_HOME:
	  zoom -= (float) 1.0f; // zoom out
		break;
    case GLUT_KEY_UP:
	 	RotateX = ((int)RotateX +2)%360; //Rotate Up around X-Axis
		break;
	case GLUT_KEY_DOWN:
		RotateX = ((int)RotateX -2)%360; // Rotate Down around X-Axis
		  break;
	case GLUT_KEY_LEFT:
    	RotateY =((int)RotateY +2)%360; // Rotate Left
		break;
	case GLUT_KEY_RIGHT:
		RotateY = ((int)RotateY -2)%360; // Rotate Left
		 break;
   }
  glutPostRedisplay(); //marks the current window as needing to be redisplayed
}

static void idle(void)
{
    glutPostRedisplay(); //marks the current window as needing to be redisplayed
}
void t_load(char* Filepath, GLuint &tex)
 {
    // int width, height;
    //unsigned char *image;
        glBindTexture(GL_TEXTURE_2D,tex); // to bind texture
        image = SOIL_load_image(Filepath, &width, &height, 0, SOIL_LOAD_RGB); // load image
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image); // load texture image
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); //back-facing

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // assign a color you like

    glEnable(GL_NORMALIZE); // normal vector is normalised to unit length
    glEnable(GL_COLOR_MATERIAL); // enable color

    glEnable(GL_DEPTH_TEST); // enable depth
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // specify implementation hints
    glShadeModel(GL_SMOOTH);

     glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE); // setup texture enviornment
        glEnable(GL_TEXTURE_2D); //enable texture
        glGenTextures(2, tex); // loading two texture
        t_load("img/c.jpg",tex[0] );// image for bunny
        t_load("img/b.jpg",tex[1] );// image of Anteneam

     // all specifications for colors properties
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
    glutInit(&argc, argv); // intilize glut lib

    glutInitWindowSize(1920,1080); // setup window size
    glutInitWindowPosition(0,0);// setup initial position
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Press '1' for ateneam or '2' for bunny");
    init();
    glutReshapeFunc(resize); // window resize
    glutDisplayFunc(display); // for display
    glutKeyboardFunc(key); // allow keyboard keys input
    glutSpecialFunc(Specialkeys); // load special keys function


    glutIdleFunc(idle);// load idle func()

    glutMainLoop();

    return EXIT_SUCCESS;
}
