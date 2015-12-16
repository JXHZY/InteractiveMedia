/*
Ying Zhou
CPS 592 Interactive Media
Assignment2
assignment2_ying.h
02/23/2015
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;

class bezierSpline{
public:
	Point pt[4];
	int finished;
	int firstLine;
	int secondLine;
	int numberPoints;
	int show;

	void initSpline()
	{
		pt[0].x = -1;
		pt[0].y = -1;
		pt[1].x = -1;
		pt[1].y = -1; 
		pt[2].x = -1;
		pt[2].y = -1;
		pt[3].x = -1;
		pt[3].y = -1;
		finished = 0;
		firstLine = 0;
		secondLine = 0;
		numberPoints = 0;
		show = 1;
	}
};