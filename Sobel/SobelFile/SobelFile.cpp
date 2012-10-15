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
	cout << (String)(char*)argv[1];
	cvNamedWindow("temp");
	cvWaitKey();
	int sobelHoriz[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
	int sobelVert[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
	char* inLocation = (char*)argv[1];
	cvWaitKey();
	IplImage* in = cvLoadImage(inLocation);
	cout << inLocation;
	Mat first(in);
	int rows = first.rows;
	int cols = first.cols;
	Mat second(rows, cols, CV_8UC3);
	cvtColor(first, second, CV_BGR2GRAY);
	Mat out(rows, cols, CV_8UC1);
	int i,j;
	uchar *line1, *line2, *line3, *lineOut;
	for (i = 1; i < rows-1; i++) {
		line1 = second.ptr<uchar>(i-1);
		line2 = second.ptr<uchar>(i);
		line3 = second.ptr<uchar>(i+1);
		lineOut = out.ptr<uchar>(i);
		for (j = 1; j < cols-1; j++) {
			int line1x = (line1[j-1] * sobelHoriz[0][0]) + (line1[j] * sobelHoriz[0][1]) + (line1[j+1] * sobelHoriz[0][2]);
			int line2x = (line2[j-1] * sobelHoriz[1][0]) + (line2[j] * sobelHoriz[1][1]) + (line2[j+2] * sobelHoriz[1][2]);
			int line3x = (line3[j-1] * sobelHoriz[2][0]) + (line3[j] * sobelHoriz[2][1]) + (line3[j+1] * sobelHoriz[2][2]);
				
			int line1y = (line1[j-1] * sobelVert[0][0]) + (line1[j] * sobelVert[0][1]) + (line1[j+1] * sobelVert[0][2]);
			int line2y = (line2[j-1] * sobelVert[1][0]) + (line2[j] * sobelVert[1][1]) + (line2[j+2] * sobelVert[1][2]);
			int line3y = (line3[j-1] * sobelVert[2][0]) + (line3[j] * sobelVert[2][1]) + (line3[j+1] * sobelVert[2][2]);

			double xgrad = line1x + line2x + line3x;
			double ygrad = line1y + line2y + line3y;
			double totalgrad = sqrt(((xgrad * xgrad) + (ygrad * ygrad)) / 32);
			lineOut[j] = (uchar)totalgrad;
		}
	}
	cvWaitKey();
	IplImage output = out;
	String filename = "Sobelized " + *inLocation;
	//char* name = filename.c_str;
	//cvSaveImage(name, &output);
	return 0;
}

