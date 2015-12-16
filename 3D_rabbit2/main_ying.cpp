/*
CPS592 Interactive Media
Assignment4
main_ying.cpp
Ying Zhou
03/24/2015
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

#include "assignment4_ying.h"
#define phongExponent 0.5
//#define phongExponent 50

using namespace std;

vector<Vertex> vt_list;
vector<Triangle> tg_list;
int whichStyle = 0;				//0 is the triangle surface view, 1 is the mesh view
int position[2];
int mousePositionChange[2];
int siganlDown = 0;
int choose = 0;				//0 means rotation, 1 means translate
int eyeStylr = 0;			//0 means world origin, 1 means object center as the origin
Vertex center;

int signal = 0;			//0 means no shading at all; 1 means only diffuse and ambient; 2 means with diffuse, ambient and phong terms
Color lighColor;
Color ambientColor;
Color phongColor;
Color objectColor;
Vertex lightPosition;
Vertex view;

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
	center.x /= vt_list.size();
	center.y /= vt_list.size();
	center.z /= vt_list.size();
}

void normal()			//compute the normal for each triangle
{
	Vertex keep1;
	Vertex keep2;
	for (vector<Triangle>::iterator iter = tg_list.begin(); iter != tg_list.end(); iter++)
	{
		keep1 = vt_list[iter->v2 - 1] - vt_list[iter->v1 - 1];
		keep2 = vt_list[iter->v3 - 1] - vt_list[iter->v1 - 1];
		iter->normalValue = keep1.cross(keep2);
	}
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
	normal();
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
	normal();
}

Color diffuse(Vertex l, Vertex n)
{
	Color result;
	l.unitCalculate();
	n.unitCalculate();
	if (n.dotProduct(l)>0)
	{
		result = objectColor*lighColor*(n.dotProduct(l));
	}
	else
	{
		result.R = 0.0;
		result.G = 0.0;
		result.B = 0.0;
	}
	return result;
}

Color ambient()
{
	Color result;
	result = objectColor * ambientColor;
	return result;
}

Color phong(Vertex l, Vertex n, Vertex v)
{
	Color result;
	Vertex r;
	float help;
	l.unitCalculate();
	n.unitCalculate();
	v.unitCalculate();
	help = n.dotProduct(l);
	r.x = 2 * n.unit_x * help - l.unit_x;
	r.y = 2 * n.unit_y * help - l.unit_y;
	r.z = 2 * n.unit_z * help - l.unit_z;
	r.unitCalculate();
	if (v.dotProduct(r) > 0)
	{
		result = lighColor * phongColor * (pow(v.dotProduct(r), phongExponent));
	}
	else
	{
		result = lighColor * phongColor * (pow(0,phongExponent));
	}
	return result;
}

Color finalResult(Color diffuseColor, Color ambientColor, Color phongColor)
{
	Color result;
	result = diffuseColor + ambientColor + phongColor;
	if (result.R > 1)
	{
		result.R = 1.0;
	}
	if (result.G > 1)
	{
		result.G = 1.0;
	}
	if (result.B > 1)
	{
		result.B = 1.0;
	}
	return result;
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
	Vertex l;
	Vertex v;
	Color keepDiffuse(0.0,0.0,0.0);
	Color keepAmbient(0.0, 0.0, 0.0);
	Color keepPhong(0.0, 0.0, 0.0);
	Color finalColor(0.0, 0.0, 0.0);

	if (whichStyle == 0)			//triangle surface view, drawing a solid triangle
	{
		arg = GL_TRIANGLES;
	}
	else
	{
		arg = GL_LINE_LOOP;
	}
	if (signal == 0)
	{
		glColor3f(objectColor.R, objectColor.G, objectColor.B);
		for (vector<Triangle>::iterator iter = tg_list.begin(); iter != tg_list.end(); iter++)
		{
			glBegin(arg);
			glVertex3f(vt_list[iter->v1 - 1].x, vt_list[iter->v1 - 1].y, vt_list[iter->v1 - 1].z); //point A
			glVertex3f(vt_list[iter->v2 - 1].x, vt_list[iter->v2 - 1].y, vt_list[iter->v2 - 1].z); //point B
			glVertex3f(vt_list[iter->v3 - 1].x, vt_list[iter->v3 - 1].y, vt_list[iter->v3 - 1].z); //point C
			glEnd();
		}
	}
	else if (signal == 1)				//only diffuse and ambient
	{
		for (vector<Triangle>::iterator iter = tg_list.begin(); iter != tg_list.end(); iter++)
		{
			glBegin(arg);
			//point A
			l = lightPosition - vt_list[iter->v1 - 1];
			keepDiffuse = diffuse(l, iter->normalValue);
			keepAmbient = ambient();
			finalColor = finalResult(keepDiffuse, keepAmbient, keepPhong);
			glColor3f(finalColor.R, finalColor.G, finalColor.B);
			glVertex3f(vt_list[iter->v1 - 1].x, vt_list[iter->v1 - 1].y, vt_list[iter->v1 - 1].z);
			//point B
			l = lightPosition - vt_list[iter->v2 - 1];
			keepDiffuse = diffuse(l, iter->normalValue);
			keepAmbient = ambient();
			finalColor = finalResult(keepDiffuse, keepAmbient, keepPhong);
			glColor3f(finalColor.R, finalColor.G, finalColor.B);
			glVertex3f(vt_list[iter->v2 - 1].x, vt_list[iter->v2 - 1].y, vt_list[iter->v2 - 1].z);
			//point C
			l = lightPosition - vt_list[iter->v3 - 1];
			keepDiffuse = diffuse(l, iter->normalValue);
			keepAmbient = ambient();
			finalColor = finalResult(keepDiffuse, keepAmbient, keepPhong);
			glColor3f(finalColor.R, finalColor.G, finalColor.B);
			glVertex3f(vt_list[iter->v3 - 1].x, vt_list[iter->v3 - 1].y, vt_list[iter->v3 - 1].z);
			glEnd();
		}
	}
	else if (signal == 2)					//diffuse, ambient and phong
	{
		for (vector<Triangle>::iterator iter = tg_list.begin(); iter != tg_list.end(); iter++)
		{
			glBegin(arg);
			//point A
			l = lightPosition - vt_list[iter->v1 - 1];
			keepDiffuse = diffuse(l, iter->normalValue);
			keepAmbient = ambient();
			v = view - vt_list[iter->v1 - 1];
			keepPhong = phong(l, iter->normalValue, v);
			finalColor = finalResult(keepDiffuse, keepAmbient, keepPhong);
			glColor3f(finalColor.R, finalColor.G, finalColor.B);
			glVertex3f(vt_list[iter->v1 - 1].x, vt_list[iter->v1 - 1].y, vt_list[iter->v1 - 1].z);
			//point B
			l = lightPosition - vt_list[iter->v2 - 1];
			keepDiffuse = diffuse(l, iter->normalValue);
			keepAmbient = ambient();
			v = view - vt_list[iter->v2 - 1];
			keepPhong = phong(l, iter->normalValue, v);
			finalColor = finalResult(keepDiffuse, keepAmbient, keepPhong);
			glColor3f(finalColor.R, finalColor.G, finalColor.B);
			glVertex3f(vt_list[iter->v2 - 1].x, vt_list[iter->v2 - 1].y, vt_list[iter->v2 - 1].z);
			//point C
			l = lightPosition - vt_list[iter->v3 - 1];
			keepDiffuse = diffuse(l, iter->normalValue);
			keepAmbient = ambient();
			v = view - vt_list[iter->v3 - 1];
			keepPhong = phong(l, iter->normalValue, v);
			finalColor = finalResult(keepDiffuse, keepAmbient, keepPhong);
			glColor3f(finalColor.R, finalColor.G, finalColor.B);
			glVertex3f(vt_list[iter->v3 - 1].x, vt_list[iter->v3 - 1].y, vt_list[iter->v3 - 1].z);
			glEnd();
		}
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
	case '0':
		signal = 0;
		glutPostRedisplay();
		break;
	case '1':
		signal = 1;
		glutPostRedisplay();
		break;
	case '2':
		signal = 2;
		glutPostRedisplay();
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

	//object color
	//yellow rubby~
	objectColor.R = 0.9;
	objectColor.G = 0.7;
	objectColor.B = 0.1;
	//little white rubby~
	//objectColor.R = 0.5;
	//objectColor.G = 0.5;
	//objectColor.B = 0.5;
	//green rubby~
	//objectColor.R = 0.0;
	//objectColor.G = 1.0;
	//objectColor.B = 0.0;

	//light color
	lighColor.R = 0.9;
	lighColor.G = 0.9;
	lighColor.B = 0.9;

	//ambient color
	ambientColor.B = 0.2;
	ambientColor.G = 0.2;
	ambientColor.R = 0.2;

	//Color phongColor(0.8, 0.8, 0.8);
	phongColor.B = 0.8;
	phongColor.G = 0.8;
	phongColor.R = 0.8;

	//light position
	lightPosition.x = 0;
	lightPosition.y = 0;
	lightPosition.z = 5;

	//view position
	view.x = -0.4;
	view.y = 0.4;
	view.z = -0.4 * .48 / .64;
	//view.x = 0.0;
	//view.y = 0.0;
	//view.z = 5.0;

	normal();
}

int main(int argc, char * argv[])
{
	//Load the "bunny_low.obj" or "bunny_high.obj" file here, for example, create a function below
	loadFile("bunny_high.obj");
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
