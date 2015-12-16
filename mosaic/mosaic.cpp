/*
Ying Zhou
CPS 592 Interactive Media
Assignment1 (Version1)
assignment1_main.cpp
02/04/2015
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

cv::Mat img, original;
cv::Point pt[2];
int blur_degree = 5;
bool first = true;

void drawMosaic()
{
	int totalNumber = 0;
	int lastx = 0;
	int lasty = 0;
	int maxy;
	int miny;
	int maxx;
	int minx;
	int totalRValue = 0;
	int totalGValue = 0;
	int totalBValue = 0;
	int avgRValue = 0;
	int avgGValue = 0;
	int avgBValue = 0;

	//It is a rectangle, need know which one big
	if (pt[1].y > pt[0].y)
	{
		maxy = pt[1].y;
		miny = pt[0].y;
	}
	else
	{
		maxy = pt[0].y;
		miny = pt[1].y;
	}
	if (pt[1].x > pt[0].x)
	{
		maxx = pt[1].x;
		minx = pt[0].x;
	}
	else
	{
		maxx = pt[0].x;
		minx = pt[1].x;
	}

	for (int y = miny; y <= maxy; y = y + blur_degree)
	{
		for (int x = minx; x <= maxx; x = x + blur_degree)
		{
			totalRValue = 0;
			totalGValue = 0;
			totalBValue = 0;
			avgRValue = 0;
			avgGValue = 0;
			avgBValue = 0;
			int by = 0;
			totalNumber = 0;
			while ((by < blur_degree) && ((y + by) <= maxy))
			{
				int bx = 0;
				while ((bx < blur_degree) && ((x + bx) <= maxx))
				{
					totalBValue = totalBValue + img.ptr<uchar>(y + by)[3 * (x + bx)];
					totalGValue = totalGValue + img.ptr<uchar>(y + by)[3 * (x + bx) + 1];
					totalRValue = totalRValue + img.ptr<uchar>(y + by)[3 * (x + bx) + 2];
					bx++;
					totalNumber++;
				}
				by++;
			}
			//the average value for this block
			avgBValue = totalBValue / totalNumber;
			avgGValue = totalGValue / totalNumber;
			avgRValue = totalRValue / totalNumber;

			//make the mosica for this block
			by = 0;
			while ((by < blur_degree) && ((y + by) <= maxy))
			{
				int bx = 0;
				while ((bx < blur_degree) && ((x + bx) <= maxx))
				{
					img.ptr<uchar>(y + by)[3 * (x + bx)] = avgBValue;
					img.ptr<uchar>(y + by)[3 * (x + bx) + 1] = avgGValue;
					img.ptr<uchar>(y + by)[3 * (x + bx) + 2] = avgRValue;
					bx++;
				}
				by++;
			}
		}
	}
}

/*Mouse callback function*/
void onMouse(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN: //left click
		if (first == true)					//the first click
		{
			pt[0].x = x;
			pt[0].y = y;
			printf("The first click at: %d, %d\n", x, y);
			first = false;
		}
		else if (first == false)			//the second click
		{
			pt[1].x = x;
			pt[1].y = y;
			printf("The Second click at: %d, %d\n", x, y);
			drawMosaic();
			first = true;
		}
		break;
	case CV_EVENT_RBUTTONDOWN: //right click
		break;
	case CV_EVENT_LBUTTONUP: //left button up
		break;
	case CV_EVENT_RBUTTONUP:
		break;
	case CV_EVENT_MOUSEMOVE:
		break;
	}
}

void originalPicture()
{
	original.copyTo(img);
	blur_degree = 5;
	first = true;
}

int main(int argc, const char * argv[]) {

	img = cv::imread("test.jpg"); //read image from hard drive
	img.copyTo(original);

	//mouse response function with the window "test"
	cv::namedWindow("Assignment1_Ying");
	cv::setMouseCallback("Assignment1_Ying", onMouse); //register your implemented

	/*Show the image*/
	for (;;)
	{
		cv::imshow("Assignment1_Ying", img);

		/* Keyboard Event Listener */
		char c = cv::waitKey(1);
		if (c == 27) //ESC
			break;
		if ((c == 'i') || (c == 'I'))		//increase the value of "blur degree"
		{
			//if "blue_degree" is less than 5, it increases by 1 each time
			if (blur_degree < 5)
			{
				printf("This is the Increasing Case:\n");
				printf("The blur_degree is: %d\n",blur_degree);
				blur_degree++;
				printf("Increasing by 1. After the increasing, the blur_degree is: %d\n",blur_degree);
			}
			//if "blue_degree" is equal to or great than 5, it increases by 5 each time
			else
			{
				printf("This is the Increasing Case:\n");
				printf("The blur_degree is: %d\n", blur_degree);
				blur_degree = blur_degree + 5;
				printf("Increasing by 5. After the increasing, the blur_degree is: %d\n", blur_degree);
			}
		}
		if ((c == 'd') || (c == 'D'))					//decrease the value of "blur degree"
		{
			//The "blue_degree" less than or equal to 1

			if ((blur_degree == 1) || (blur_degree < 1))
			{
				printf("This is the Decreasing Case!!!The blur_degree is 1!!\n");
				printf("Please do the increasing first!\n");
			}
			//if "blue_degree" is less than 5, it increases by 1 each time
			else if ((blur_degree > 1) && (blur_degree < 5))
			{
				printf("This is the Decreasing Case:\n");
				printf("The blur_degree is: %d\n", blur_degree);
				blur_degree--;
				printf("Decreasing by 1. After the decreasing, the blur_degree is: %d\n", blur_degree);
			}
			//if "blue_degree" is equal to or great than 5, it increases by 5 each time
			else if (blur_degree >5)
			{
				printf("This is the Decreasing Case:\n");
				printf("The blur_degree is: %d\n", blur_degree);
				blur_degree = blur_degree-5;
				printf("Decreasing by 1. After the decreasing, the blur_degree is: %d\n", blur_degree);
			}
		}
		if ((c == 's') || (c == 'S'))			//save the result image into a local file
		{
			cv::imwrite("result.jpg", img);
			printf("Already save the picture!\n");
		}
		if ((c == 'r') || (c == 'R'))			//remove the current editing results and return  to the original image
		{
			originalPicture();
			printf("The picture return to the original state!\n");
		}
	}
	return 0;
}
