// Sobel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

void sobel(IplImage* inputIpl, int sobelHoriz[3][3], int sobelVert[3][3], String inLocation);

int _tmain(int argc, _TCHAR* argv[]) {
	bool repeat = true;
	int sobelHoriz[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
	int sobelVert[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
	while (repeat) {
		cout << "Input the file name. Maintains your selected file extension. \n";
		String inL;
		cin >> inL;
		cin;
		char* fileLoc = (char*)inL.c_str();
		cvWaitKey();
		IplImage* frame = cvLoadImage(fileLoc);
		sobel(frame, sobelHoriz, sobelVert, inL);
		cout << "Do you want to process another image? [y/n] \n";
		String response;
		cin >> response;
		if (response == "n" || response == "N") {
			repeat = false;
		}
	}
	return 0;
}

void sobel(IplImage* inputIpl, int sobelHoriz[3][3], int sobelVert[3][3], String inLocation) {
	Mat input(inputIpl);
	Mat red(input.rows, input.cols, CV_8UC1);
	Mat green(input.rows, input.cols, CV_8UC1);
	Mat blue(input.rows, input.cols, CV_8UC1);
	Mat redOut(input.rows, input.cols, CV_8UC3);
	Mat greenOut(input.rows, input.cols, CV_8UC3);
	Mat blueOut(input.rows, input.cols, CV_8UC3);
	Mat out(input.rows, input.cols, CV_8UC3);
	for (int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			Vec3b in = input.at<Vec3b>(i,j);
			blue.at<uchar>(i,j) = in[0];
			green.at<uchar>(i,j) = in[1];
			red.at<uchar>(i,j) = in[2];

		}
	}


	int i,j;
	uchar *line1, *line2, *line3;
	Vec3b *lineOut;
	for (i = 1; i < input.rows - 1; i++) {
		line1 = red.ptr<uchar>(i-1);
		line2 = red.ptr<uchar>(i);
		line3 = red.ptr<uchar>(i+1);
		lineOut = redOut.ptr<Vec3b>(i);
		for (j = 1; j < input.cols-1; j++) {
			int line1x = (line1[j-1] * sobelHoriz[0][0]) + (line1[j] * sobelHoriz[0][1]) + (line1[j+1] * sobelHoriz[0][2]);
			int line2x = (line2[j-1] * sobelHoriz[1][0]) + (line2[j] * sobelHoriz[1][1]) + (line2[j+2] * sobelHoriz[1][2]);
			int line3x = (line3[j-1] * sobelHoriz[2][0]) + (line3[j] * sobelHoriz[2][1]) + (line3[j+1] * sobelHoriz[2][2]);
				
			int line1y = (line1[j-1] * sobelVert[0][0]) + (line1[j] * sobelVert[0][1]) + (line1[j+1] * sobelVert[0][2]);
			int line2y = (line2[j-1] * sobelVert[1][0]) + (line2[j] * sobelVert[1][1]) + (line2[j+2] * sobelVert[1][2]);
			int line3y = (line3[j-1] * sobelVert[2][0]) + (line3[j] * sobelVert[2][1]) + (line3[j+1] * sobelVert[2][2]);

			double xgrad = line1x + line2x + line3x;
			double ygrad = line1y + line2y + line3y;
			double totalgrad = sqrt(((xgrad * xgrad) + (ygrad * ygrad)) / 32);
			lineOut[j][0] = 0;
			lineOut[j][1] = 0;
			lineOut[j][2] = (uchar)totalgrad;
		}
	}

	for (i = 1; i < input.rows - 1; i++) {
		line1 = blue.ptr<uchar>(i-1);
		line2 = blue.ptr<uchar>(i);
		line3 = blue.ptr<uchar>(i+1);
		lineOut = blueOut.ptr<Vec3b>(i);
		for (j = 1; j < input.cols-1; j++) {
			int line1x = (line1[j-1] * sobelHoriz[0][0]) + (line1[j] * sobelHoriz[0][1]) + (line1[j+1] * sobelHoriz[0][2]);
			int line2x = (line2[j-1] * sobelHoriz[1][0]) + (line2[j] * sobelHoriz[1][1]) + (line2[j+2] * sobelHoriz[1][2]);
			int line3x = (line3[j-1] * sobelHoriz[2][0]) + (line3[j] * sobelHoriz[2][1]) + (line3[j+1] * sobelHoriz[2][2]);
				
			int line1y = (line1[j-1] * sobelVert[0][0]) + (line1[j] * sobelVert[0][1]) + (line1[j+1] * sobelVert[0][2]);
			int line2y = (line2[j-1] * sobelVert[1][0]) + (line2[j] * sobelVert[1][1]) + (line2[j+2] * sobelVert[1][2]);
			int line3y = (line3[j-1] * sobelVert[2][0]) + (line3[j] * sobelVert[2][1]) + (line3[j+1] * sobelVert[2][2]);

			double xgrad = line1x + line2x + line3x;
			double ygrad = line1y + line2y + line3y;
			double totalgrad = sqrt(((xgrad * xgrad) + (ygrad * ygrad)) / 32);
			lineOut[j][0] = (uchar)totalgrad;
			lineOut[j][1] = 0;
			lineOut[j][2] = 0;
		}
	}

	for (i = 1; i < input.rows - 1; i++) {
		line1 = green.ptr<uchar>(i-1);
		line2 = green.ptr<uchar>(i);
		line3 = green.ptr<uchar>(i+1);
		lineOut = greenOut.ptr<Vec3b>(i);
		for (j = 1; j < input.cols-1; j++) {
			int line1x = (line1[j-1] * sobelHoriz[0][0]) + (line1[j] * sobelHoriz[0][1]) + (line1[j+1] * sobelHoriz[0][2]);
			int line2x = (line2[j-1] * sobelHoriz[1][0]) + (line2[j] * sobelHoriz[1][1]) + (line2[j+2] * sobelHoriz[1][2]);
			int line3x = (line3[j-1] * sobelHoriz[2][0]) + (line3[j] * sobelHoriz[2][1]) + (line3[j+1] * sobelHoriz[2][2]);
				
			int line1y = (line1[j-1] * sobelVert[0][0]) + (line1[j] * sobelVert[0][1]) + (line1[j+1] * sobelVert[0][2]);
			int line2y = (line2[j-1] * sobelVert[1][0]) + (line2[j] * sobelVert[1][1]) + (line2[j+2] * sobelVert[1][2]);
			int line3y = (line3[j-1] * sobelVert[2][0]) + (line3[j] * sobelVert[2][1]) + (line3[j+1] * sobelVert[2][2]);

			double xgrad = line1x + line2x + line3x;
			double ygrad = line1y + line2y + line3y;
			double totalgrad = sqrt(((xgrad * xgrad) + (ygrad * ygrad)) / 32);
			lineOut[j][0] = 0;
			lineOut[j][1] = (uchar)totalgrad;
			lineOut[j][2] = 0;
		}
	}

	for (i = 1; i < input.rows - 1; i++) {
		for (j = 1; j < input.cols - 1; j++) {
			out.at<Vec3b>(i,j)[0] = blueOut.at<Vec3b>(i,j)[0];
			out.at<Vec3b>(i,j)[1] = greenOut.at<Vec3b>(i,j)[1];
			out.at<Vec3b>(i,j)[2] = redOut.at<Vec3b>(i,j)[2];
		}
	}
	//IplImage redFile = redOut;
	//IplImage blueFile = blueOut;
	//IplImage greenFile = greenOut;
	IplImage output = out;
	String outL = "sobel-" + inLocation;
	char* fileLocation = (char*)outL.c_str();
	cvSaveImage(fileLocation, &output);
	//cvSaveImage(fileLocation + *"_red.jpg", &redFile);
	//cvSaveImage(fileLocation + *"_green.jpg", &greenFile);
	//cvSaveImage(fileLocation + *"_blue.jpg", &blueFile);
}