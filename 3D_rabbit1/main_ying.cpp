/*
CPS592 Interactive Media
Assignment3
main_ying.cpp
Ying Zhou
03/16/2015
*/

#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

using namespace std;

#include "assignment3_ying.h"

vector<Vertex> vt_list;
vector<Triangle> tg_list;
int whichStyle = 0;				//0 is the triangle surface view, 1 is the mesh view
int position[2];
int mousePositionChange[2];
int siganlDown = 0;
int choose = 0;				//0 means rotation, 1 means translate
int eyeStylr = 0;			//0 means world origin, 1 means object center as the origin
Vertex center;

void loadFile(string s)
{
	string singal;
	ifstream fin(s);
	while (!fin.eof())
	{
		fin >> singal;
		float a, b, c;
		int v1, v2, v3;
		Vertex keepTriangle;
		if (singal == "v")
		{
			fin >> a >> b >> c;
			Vertex keepVertex(a, b, c);
			vt_list.push_back(keepVertex);
		}
		if (singal == "f")
		{
			fin >> v1 >> v2 >> v3;
			Triangle keepTriangle(v1, v2, v3);
			tg_list.push_back(keepTriangle);
		}
	}
}

void findCenter()
{
	/*Find object center*/
	for (vector<Vertex>::iterator iter = vt_list.begin(); iter != vt_list.end(); iter++)
	{
		center.x = center.x + iter->x;
		center.y = center.y + iter->y;
		center.z = center.z + iter->z;
	}
	//cout << center.x << "  " << center.y << "  " << center.z << endl;
	center.x /= vt_list.size();
	center.y /= vt_list.size();
	center.z /= vt_list.size();
}

void translate()
{
	Mat T;
	float tx = mousePositionChange[0]/500.f;     
	float ty = -(mousePositionChange[1]/500.f);
	float tz = 0;
	T.elem[0][0] = 1.0f;
	T.elem[0][3] = tx;
	T.elem[1][1] = 1.0f;
	T.elem[1][3] = ty; 
	T.elem[2][2] = 1.0f;
	T.elem[2][3] = tz;
	T.elem[3][3] = 1.0f;
	
	for (vector<Vertex>::iterator iter = vt_list.begin(); iter != vt_list.end(); iter++)
	{
		*iter = T*(*iter);
	}
}

void rotate()
{
	float tx = mousePositionChange[1] / 100.f;
	float ty = mousePositionChange[0] / 100.f;
	//float tz = 0;

	Mat Rx;
	Mat Ry;
	//Mat Rz;
	Mat P;
	Mat T;
	Mat Tr;

	//Rotate about x-axis
	Rx.elem[0][0] = 1.0f;
	Rx.elem[1][1] = cos(tx);
	Rx.elem[1][2] = -sin(tx);
	Rx.elem[2][1] = sin(tx);
	Rx.elem[2][2] = cos(tx);
	Rx.elem[3][3] = 1.0f;

	//Rotate about y-axis
	Ry.elem[0][0] = cos(ty);
	Ry.elem[0][2] = sin(ty);
	Ry.elem[1][1] = 1;
	Ry.elem[2][0] = -sin(ty);
	Ry.elem[2][2] = cos(ty);
	Ry.elem[3][3] = 1.0f;

	//Rotate about z-axis
	//Rz.elem[0][0] = cos(tz);
	//Rz.elem[0][1] = -sin(tz);
	//Rz.elem[1][0] = sin(tz);
	//Rz.elem[1][1] = cos(tz);
	//Rz.elem[2][2] = 1.0f;
	//Rz.elem[3][3] = 1.0f;

	if (eyeStylr == 0)
	{
		//P = Rx*Ry*Rz;
		P = Rx*Ry;
		for (vector<Vertex>::iterator iter = vt_list.begin(); iter != vt_list.end(); iter++)
		{
			*iter = P * (*iter);
		}
	}
	else
	{
		findCenter();
		//T
		T.elem[0][0] = 1.0f;
		T.elem[0][3] = 0 - center.x;
		T.elem[1][1] = 1.0f;
		T.elem[1][3] = 0 - center.y;
		T.elem[2][2] = 1.0f;
		T.elem[2][3] = 0 - center.z;
		T.elem[3][3] = 1.0f;
		//Tr
		Tr.elem[0][0] = 1.0f;
		Tr.elem[0][3] = center.x;
		Tr.elem[1][1] = 1.0f;
		Tr.elem[1][3] = center.y;
		Tr.elem[2][2] = 1.0f;
		Tr.elem[2][3] = center.z;
		Tr.elem[3][3] = 1.0f;
		//P = Tr * Rx * Ry * Rz * T;
		P = Tr * Rx * Ry * T;
		for (vector<Vertex>::iterator iter = vt_list.begin(); iter != vt_list.end(); iter++)
		{
			*iter = P * (*iter);
		}
	}
}

void onMouse(int button, int state, int x, int y)
{
	GLint specialKey = glutGetModifiers();
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) 
		{	
			position[0] = x;
			position[1] = y;
			siganlDown = 1;
			if (specialKey == GLUT_ACTIVE_SHIFT) //Translate the 3D object
			{
				choose = 1;
			}
			if (specialKey == GLUT_ACTIVE_CTRL)
			{
				eyeStylr = 1;
			}
		}
		else if (state == GLUT_UP)
		{
			siganlDown = 0;
			choose = 0;
			eyeStylr = 0;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)

			break;

	default:
		break;

	}


	glutPostRedisplay(); //This is an important function to let the window re-rendered. If you do not call this function explicitly
	//the window never get refereshed. So you should call this function in the onKeyboard event as well if you
	//want to refresh the display

}

void onMouseMotion(int x, int y)
{
	if (siganlDown == 1)
	{
		mousePositionChange[0] = x - position[0];
		mousePositionChange[1] = y - position[1];
		position[0] = x;
		position[1] = y;
		if (choose == 1)		//Translate the 3D object
		{
			translate();
		}
		else					//Rotate the 3D object
		{
			rotate();
		}
	}
	glutPostRedisplay(); //This is an important function to let the window re-rendered. If you do not call this function explicitly
	//the window never get refereshed. So you should call this function in the onKeyboard event as well if you
	//want to refresh the display
}

void renderAllTriangles()
{
	int arg;
	Triangle triangleKeep;

	glColor3f(1.0, 1.0, 1.0); //RGB    little white rubby~
	if (whichStyle == 0)			//triangle surface view, drawing a solid triangle
	{
		arg = GL_TRIANGLES;
	}
	else
	{
		arg = GL_LINE_LOOP;
	}
	for (vector<Triangle>::iterator iter = tg_list.begin(); iter != tg_list.end(); iter++)
	{
		glBegin(arg);
		glVertex3f(vt_list[iter->v1 - 1].x, vt_list[iter->v1 - 1].y, vt_list[iter->v1 - 1].z); //point A
		glVertex3f(vt_list[iter->v2 - 1].x, vt_list[iter->v2 - 1].y, vt_list[iter->v2 - 1].z); //point B
		glVertex3f(vt_list[iter->v3 - 1].x, vt_list[iter->v3 - 1].y, vt_list[iter->v3 - 1].z); //point C
		glEnd();
	}
}

/*Define the 3D objects that want to show*/
void onDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderAllTriangles();
	glFlush(); //clear the memory
}

void onKeyboard(unsigned char key, int x, int y)
{
	//int imageNumber;
	switch (key)
	{
	case 27:
		exit(1);
		break;
	case 'm':
		if (whichStyle == 0)
		{
			whichStyle = 1;
			glutPostRedisplay();
		}
		else
		{
			whichStyle = 0;
			glutPostRedisplay();
		}
		break;
	case 'M':
		if (whichStyle == 0)
		{
			whichStyle = 1;
			glutPostRedisplay();
		}
		else
		{
			whichStyle = 0;
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void initialize(int argc, char * argv[])
{
	/*Initialize glut stuff*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Bunny");

	/*Initialize gl stuff*/
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.4, 0.4, -0.4 * .48 / .64, 0.4 * .48 / .64, 2, 10); //parallel projection
	gluLookAt(0, 0, 5, 0, 0.2, 0, 0, 1, 0);
}

int main(int argc, char * argv[])
{
	//Load the "bunny_low.obj" or "bunny_high.obj" file here, for example, create a function below
	loadFile("bunny_low.obj");
	//loadFile("bunny_high.obj", vt_list, tg_list);

	//initialize(); //You may creat an "initialize()" function here if there are any variables need to be assigned    
	initialize(argc,argv);

	/*Register GL stuff with the GLUT*/
	glutDisplayFunc(onDisplay);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMouseMotion);
	glutKeyboardFunc(onKeyboard);

	/*Initialize the loop*/
	glutMainLoop();

	return 0;
}
