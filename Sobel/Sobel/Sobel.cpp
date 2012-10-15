// Sobel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

int _tmain(int argc, _TCHAR* argv[]) {
	CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
	cvNamedWindow("XGrad", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("YGrad", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Grayscale", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Sobel", CV_WINDOW_AUTOSIZE);
	int sobelHoriz[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
	int sobelVert[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
	IplImage* frame = cvQueryFrame(capture);
	Mat first(frame);
	int rows = first.rows;
	int cols = first.cols;
	
	while (1) {
		frame = (cvQueryFrame(capture));
		first = frame;
		
		Mat temp;
		cvtColor(first, temp, CV_BGR2GRAY);
		flip(temp, first, 1);
		IplImage output = first;
		cvShowImage("Grayscale", &output);
		Mat out(rows, cols, CV_8UC1);
		Mat gradx(rows, cols, CV_8UC1);
		Mat grady(rows, cols, CV_8UC1);
		int i,j;
		uchar *line1, *line2, *line3, *lineOut, *lineOutX, *lineOutY;
		for (i = 1; i < rows-1; i++) {
			line1 = first.ptr<uchar>(i-1);
			line2 = first.ptr<uchar>(i);
			line3 = first.ptr<uchar>(i+1);
			lineOut = out.ptr<uchar>(i);
			lineOutX = gradx.ptr<uchar>(i);
			lineOutY = grady.ptr<uchar>(i);
			for (j = 1; j < cols-1; j++) {
				int line1x = (line1[j-1] * sobelHoriz[0][0]) + (line1[j] * sobelHoriz[0][1]) + (line1[j+1] * sobelHoriz[0][2]);
				int line2x = (line2[j-1] * sobelHoriz[1][0]) + (line2[j] * sobelHoriz[1][1]) + (line2[j+2] * sobelHoriz[1][2]);
				int line3x = (line3[j-1] * sobelHoriz[2][0]) + (line3[j] * sobelHoriz[2][1]) + (line3[j+1] * sobelHoriz[2][2]);
				
				int line1y = (line1[j-1] * sobelVert[0][0]) + (line1[j] * sobelVert[0][1]) + (line1[j+1] * sobelVert[0][2]);
				int line2y = (line2[j-1] * sobelVert[1][0]) + (line2[j] * sobelVert[1][1]) + (line2[j+2] * sobelVert[1][2]);
				int line3y = (line3[j-1] * sobelVert[2][0]) + (line3[j] * sobelVert[2][1]) + (line3[j+1] * sobelVert[2][2]);

				double xgrad = line1x + line2x + line3x;
				double ygrad = line1y + line2y + line3y;
				lineOutX[j] = abs(xgrad / 4);
				lineOutY[j] = abs(ygrad / 4);
				double totalgrad = sqrt(((xgrad * xgrad) + (ygrad * ygrad)) / 32);
				lineOut[j] = totalgrad;
			}
		}
		IplImage gradient = gradx;
		cvShowImage("XGrad", &gradient);
		gradient = grady;
		cvShowImage("YGrad", &gradient);
		output = out;
		cvShowImage("Sobel", &output);
		if (cvWaitKey(10) > -1) {
			break;
		}
	}
	cvReleaseCapture( &capture );
	cvDestroyWindow( "Graycale" );
	cvDestroyWindow("Sobel");
	return 0;
}

