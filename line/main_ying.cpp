/*
Ying Zhou
CPS 592 Interactive Media
Assignment2
main_ying.cpp
02/23/2015
*/

#include "assignment2_ying.h"

using namespace cv;
using namespace std;

Mat img_assignment2(640, 640, CV_8UC3);
Mat img_without(640, 640, CV_8UC3);
Mat img_orign(640, 640, CV_8UC3);

vector<bezierSpline> spline;
bezierSpline helpInc;
int click_number = 0;
int number;
int success = 0;
int flag = 0;				//flag==0:not drag, flag==1:drag
vector<bezierSpline>::iterator keep;
vector<bezierSpline>::iterator keep1;
vector<bezierSpline>::iterator help;

/*----------------------------------- draw point----------------------------------------------*/
void drawPoint(Point pt)
{
	cv::circle(img_assignment2, pt, 2, cv::Scalar(0, 0, 255), 2);
}

/*----------------------------------- draw bezier curve----------------------------------------------*/
void drawBezierCurve(Point cur[4])
{
	/* Set x_y matrix*/
	cv::Mat x_y(4, 2, CV_32FC1);
	x_y.ptr<float>(0)[0] = cur[0].x;
	x_y.ptr<float>(0)[1] = cur[0].y;
	x_y.ptr<float>(1)[0] = cur[1].x;
	x_y.ptr<float>(1)[1] = cur[1].y;
	x_y.ptr<float>(2)[0] = cur[2].x;
	x_y.ptr<float>(2)[1] = cur[2].y;
	x_y.ptr<float>(3)[0] = cur[3].x;
	x_y.ptr<float>(3)[1] = cur[3].y;

	/*Set M_Beizer matrix*/
	float data[4][4] = { { -1, 3, -3, 1 }, { 3, -6, 3, 0 }, { -3, 3, 0, 0 }, { 1, 0, 0, 0 } };
	cv::Mat M_Beizer(4, 4, CV_32FC1, &data);

	cv::Mat coefficient_AB(4, 2, CV_32FC1);

	coefficient_AB = M_Beizer * x_y;

	for (float t = 0.0f; t <= 1.0f; t += 0.00001f)
	{
		int x = coefficient_AB.ptr<float>(0)[0] * pow(t, 3) + coefficient_AB.ptr<float>(1)[0] * pow(t, 2)
			+ coefficient_AB.ptr<float>(2)[0] * t + coefficient_AB.ptr<float>(3)[0];


		int y = coefficient_AB.ptr<float>(0)[1] * pow(t, 3) + coefficient_AB.ptr<float>(1)[1] * pow(t, 2)
			+ coefficient_AB.ptr<float>(2)[1] * t + coefficient_AB.ptr<float>(3)[1];

		cv::circle((img_assignment2), cv::Point(x, y), 1, cv::Scalar(0, 255, 0), 1);
		cv::circle((img_without), cv::Point(x, y), 1, cv::Scalar(0, 255, 0), 1);
	}
}

/*----------------------------------- --------Drawing------------------------------------------------*/
void drawing()
{
	img_orign.copyTo(img_assignment2);
	img_orign.copyTo(img_without);
	for (vector<bezierSpline>::iterator it = spline.begin(); it != spline.end(); it++)
	{
		if (it->show == 1)
		{
			int n = it->numberPoints;
			for (int i = 0; i <= n-1; i++)
			{
				drawPoint(it->pt[i]);
			}

			if (it->firstLine == 1)
			{
				line(img_assignment2, it->pt[0], it->pt[1], Scalar(0, 0, 255));
			}

			if (it->secondLine == 1)
			{
				line(img_assignment2, it->pt[2], it->pt[3], Scalar(0, 0, 255));
			}
		}

		if (it->finished == 1)
		{
			drawBezierCurve(it->pt);
		}
	}
}

/*----------------------------------- Mouse Event----------------------------------------------*/
void onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONUP)
	{
		success = 0;
		if (flag != 1)
		{
			switch (click_number)
			{
			case 0:
				helpInc.initSpline();
				spline.push_back(helpInc);
				keep = spline.end() - 1;
				keep->pt[0].x = x;
				keep->pt[0].y = y;
				keep->numberPoints++;
				drawPoint(keep->pt[0]);
				click_number++;
				break;
			case 1:
				keep = spline.end() - 1;
				keep->pt[1].x = x;
				keep->pt[1].y = y;
				keep->firstLine = 1;
				keep->numberPoints++;
				drawPoint(keep->pt[1]);
				line(img_assignment2, keep->pt[0], keep->pt[1], Scalar(0, 0, 255));
				click_number++;
				break;
			case 2:
				keep = spline.end() - 1;
				keep->pt[2].x = x;
				keep->pt[2].y = y;
				keep->numberPoints++;
				drawPoint(keep->pt[2]);
				click_number++;
				break;
			case 3:
				keep = spline.end() - 1;
				keep->pt[3].x = x;
				keep->pt[3].y = y;
				keep->numberPoints++;
				keep->secondLine = 1;
				keep->finished = 1;
				drawPoint(keep->pt[3]);
				line(img_assignment2, keep->pt[2], keep->pt[3], Scalar(0, 0, 255));
				drawBezierCurve(keep->pt);
				click_number = 0;
				break;
			}
		}
		else
			flag = 0;
	}
	if (event == CV_EVENT_MOUSEMOVE && (flags == CV_EVENT_FLAG_LBUTTON))	
	{
		printf("Drag!!!!!!!!!!!!!!\n");
		flag = 1;
		if (success == 0)
		{
			for (vector<bezierSpline>::iterator it = spline.begin(); it != spline.end(); it++)
			{
				if ((it->pt[1].x <= x + 10) && (it->pt[1].x >= x - 10) && (it->pt[1].y <= y + 10) && (it->pt[1].y >= y - 10))
				{
					success = 1;
					help = it;
				}
				if ((it->pt[2].x <= x + 10) && (it->pt[2].x >= x - 10) && (it->pt[2].y <= y + 10) && (it->pt[2].y >= y - 10))
				{
					success = 2;
					help = it;
				}
			}
		}
		if (success == 1)
		{
			printf("Drag Right!!!!!!!!!!!!!!\n");
			help->pt[1].x = x;
			help->pt[1].y = y;
			drawing();
		}
		if (success == 2)
		{
			printf("Drag Right!!!!!!!!!!!!!!\n");
			help->pt[2].x = x;
			help->pt[2].y = y;
			drawing();
		}
	}
	if (event == CV_EVENT_RBUTTONDOWN)
	{
		if (!spline.empty())
		{
			keep1 = spline.end() - 1;
			number = keep1->numberPoints;
			keep1->pt[number - 1].x = -1;
			keep1->pt[number - 1].y = -1;
			keep1->numberPoints--;
			if (click_number == 0)
			{
				click_number = 3;
			}
			else
			{
				click_number--;
			}
			if (number == 1)
			{
				spline.pop_back();
			}
			else
			{
				if (number == 2)
				{
					keep1->firstLine = 0;
				}
				if (number == 4)
				{
					keep1->secondLine = 0;
					keep1->finished = 0;
				}
			}
			drawing();
		}
		else
		{
			printf("There is nothing in the canvas\n");
		}
	}
}

/*----------------------------------- -------- main ---------------------------------------------------*/
int main(int argc, const char * argv[])
{
	cv::namedWindow("Assignment2_ying");
	cv::setMouseCallback("Assignment2_ying", onMouse);

	while (1)
	{
		cv::imshow("Assignment2_ying", img_assignment2);
		drawing();
		char c = cv::waitKey(1);
		if (c == 27)
			break;
		if ((c == 'r') || (c == 'R'))
		{
			img_without.copyTo(img_assignment2);
			for (vector<bezierSpline>::iterator it = spline.begin(); it != spline.end(); it++)
			{
				it->show = 0;
			}
		}
		if ((c == 's') || (c == 'S'))
		{
			cv::imwrite("assignment2_result.jpg", img_assignment2);
			printf("Already save the picture!\n");
		}
	}
	return 0;
}