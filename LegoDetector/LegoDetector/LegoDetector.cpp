// LegoDetector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace cv;

Mat detectObjects(Mat input, int start, int dx);
IplImage* getImage(string name);
Mat getImage(IplImage* base, int threshold, CvCapture* capture);
Mat diff(Mat base, Mat next, int threshold); // Takes grayscale images
Mat fixPixels(Mat& input, int oldVal, int newVal);
int numColors(Mat input, int index, int dx, int upperBound);
bool isPresent(Mat input, int color);
vector<Mat> objects(Mat input, int colors, int dx, int startVal);

int _tmain(int argc, _TCHAR* argv[]) {
	int startVal = 25;
	int threshold = 30;
	int dx = 5;
	IplImage* firstImage = getImage("Baseline");
	Mat first(firstImage);
	IplImage* outputImage;
	Mat diffed;
	CvCapture *capture = cvCaptureFromCAM(CV_CAP_ANY);
	cvNamedWindow("Difference");
	cvNamedWindow("Objects");
	while (1) {
		diffed = getImage(firstImage, threshold, capture);
		outputImage = &(IplImage)diffed;
		cvShowImage("Difference", outputImage);
		Mat objects = detectObjects(diffed,startVal, dx);
		outputImage = &(IplImage)objects;
		cvShowImage("Objects", outputImage);
		if ((cvWaitKey(10) & 255) == 32) break;
	}
	cvDestroyWindow("Difference");
	cvDestroyWindow("Objects");
	//int colors = numColors(objects, startVal, dx, 255);

	cvWaitKey();
	return 0;
}

int numColors(Mat input, int start, int dx, int upperBound) {
	int output = 0;
	for (int i = start; i <= upperBound; i += dx) {
		if (isPresent(input, i)) {
			output++;
		}
	}
	return output;
}

bool isPresent(Mat input, int color) {
	uchar* row;
	for (int i = 0; i < input.rows; i++) {
		row = input.ptr<uchar>(i);
		for (int j = 0; j < input.cols; j++) {
			if (row[j] == color) {
				return true;
			}
		}
	}
	return false;
}

IplImage* getImage(string name) { // gets and outputs an image; returns on spacebar
	CvCapture *capture = cvCaptureFromCAM(CV_CAP_ANY);
	IplImage *output;
	while (1) {
		output = cvQueryFrame(capture);
		cvShowImage(name.c_str(), output);
		if ((cvWaitKey(10) & 255) == 32) break;
	}
	output = cvCloneImage(output);
	cvReleaseCapture(&capture);
	cvDestroyWindow(name.c_str());
	return output;
}

Mat getImage(IplImage* base, int threshold, CvCapture* capture) { // gets and outputs a diff based on baseline image; returns on spacebar
	IplImage* next = cvQueryFrame(capture);
	Mat baseline(base);
	cvtColor(baseline, baseline, CV_BGR2GRAY);
	Mat temp(next);
	cvtColor(temp, temp, CV_BGR2GRAY);
	Mat diffedImage = diff(baseline, temp, threshold);
	return diffedImage;
}

Mat diff(Mat base, Mat next, int threshold) {
	Mat output(base.rows, base.cols, CV_8UC1);
	for (int i = 0; i < base.rows; i++) {
		uchar* baseRow = base.ptr<uchar>(i);
		uchar* nextRow = next.ptr<uchar>(i);
		uchar* outputRow = output.ptr<uchar>(i);
		for (int j = 0; j < base.cols; j++) {
			int difference = abs(baseRow[j] - nextRow[j]);
			if (difference < threshold) {
				outputRow[j] = 0;
			} else {
				outputRow[j] = 255;
			}
		}
	}
	return output;
}

Mat detectObjects(Mat input, int index, int dx) {
	Mat output(input.rows, input.cols, CV_8UC1);
	uchar* inputRow;
	uchar* outputRow;
	for (int i = 0; i < input.rows; i++) {
		inputRow = input.ptr<uchar>(i);
		outputRow = output.ptr<uchar>(i);
		for (int j = 0; j < input.cols; j++) {
			if (inputRow[j] == 0) {
				outputRow[0] = 0;
			} else {
				uchar left;
				uchar above;
				if (j > 0 && inputRow[j-1] != 0) { // has a left
					left = inputRow[j-1];
					outputRow[j] = left;
					//index = left;
					if (i > 0) { // has an above
						above = input.at<uchar>(i-1, j);
						if (above != 0) { // above is set
							fixPixels(input, outputRow[j], above);
							//index = above;
						} else { // above not set
							// do nothing
						}
					} else { // has no above
						// do nothing
					}
				} else { // no left or not set
					if (i > 0) { // has an above
						above = input.at<uchar>(i-1, j);
						if (above != 0) { // above is set
							outputRow[j] = above;
							//index = above;
						} else { // above is not set
							index += dx;
							outputRow[j] = index;
						}
					} else { // has no above
						index += dx;
						outputRow[j] = index;
					}
				}
			}
		}
	}
	return output;
}

Mat fixPixels(Mat& input, int oldVal, int newVal) {
	uchar* row;
	for (int i = 0; i < input.rows; i++) {
		row = input.ptr<uchar>(i);
		for (int j = 0; j < input.cols; j++) {
			if (row[j] == oldVal) {
				row[j] = newVal;
			}
		}
	}
	return input;
}