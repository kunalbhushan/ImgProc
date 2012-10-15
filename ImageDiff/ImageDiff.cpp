// ImageDiff.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
using namespace cv;

Mat detectObjects(Mat input);
Mat fixPixels(Mat& output, int oldVal, int newVal);
Mat centroid(Mat& input, vector<int> colors);
Mat diff(Mat first, Mat second, int minLevel);
vector<int> colorsPresent(Mat output);
int* centroidColor(Mat input, int color);
Mat drawX(Mat& input, int x, int y);
int centralMoment(Mat input, int color, int p, int q);
vector<Mat> objects(Mat input, vector<double> degs, int& limits, vector<int> colors);
vector<double> angles(Mat input, vector<int> colors);
Mat rotateImage(Mat& source, double angle);
Mat smallFry(Mat& input, vector<int> colorQuants, int threshold);
vector<int> colorQuantities(Mat& input, vector<int> colors);

int _tmain(int argc, _TCHAR* argv[]) {
	CvCapture *capture;
	capture = cvCaptureFromCAM( CV_CAP_ANY );
	IplImage *firstCapture;
	IplImage *secondCapture;
	cvNamedWindow("Difference");
	while (1) {
		firstCapture = cvQueryFrame(capture);
		cvShowImage("Difference", firstCapture);
		if ( (cvWaitKey(10) & 255) == 32 ) break;
	}
	firstCapture = cvQueryFrame(capture);
	firstCapture = cvCloneImage(firstCapture);
	Mat first(firstCapture);
	cvShowImage("Difference", firstCapture);
	int rows = first.rows;
	int cols = first.cols;
	int minLevel = 30;
	Mat diffedImage;
	Mat second;
	while (1) {
		secondCapture = cvQueryFrame(capture);
		second = secondCapture;
		diffedImage = diff(first, second, minLevel);
		IplImage out = diffedImage;
		cvShowImage("Difference", &out);
		if ( (cvWaitKey(10) & 255) == 32 ) {
			cvSaveImage("objects.png", secondCapture);
			cvReleaseCapture(&capture);
			break;
		}
	}
	// detect the objects
	Mat objectsImage = detectObjects(diffedImage);
	vector<int> colors;
	colors = colorsPresent(objectsImage);/*
	int* quants = colorQuantities(diffedImage, colors);
	diffedImage = smallFry(diffedImage, quants, 5);
	colors = colorsPresent(diffedImage);*/
	IplImage out;
	out	= objectsImage;
	cvShowImage("Difference", &out);
	cvWaitKey();
	// get the degree measures and separate the objects, then centroid them yeeeeeaaaaaahhhhhhh
	vector<double> degrees = angles(objectsImage, colors);
	int imageMax = 4;
	//second = cvLoadImage("objects.png");
	vector<Mat> obs = objects(objectsImage, degrees, imageMax, colors);
	for (int i = 0; i < imageMax; i++) {
		Mat temp = obs.back();
		obs.pop_back();
		//temp = centroid(temp, colors);
		out = temp;
		char name[9] = "Object ";
		sprintf_s(name, "Object %d", i);
		cvSaveImage("derp.png", &out);
		cvShowImage(name, &out);
	}
	//delete [] quants;

	cvWaitKey();
	cvDestroyWindow("Difference");

	// centroid the whole diff
	diffedImage = centroid(objectsImage, colors);
	out = diffedImage;
	cvShowImage("Centroids", &out);
	cvSaveImage("image.png", &out);
	cvWaitKey();
	cvReleaseImage(&firstCapture);
	cvDestroyWindow("Centroids");
	for (int i = 0; i < imageMax; i++) {
		cvDestroyWindow("Object " + i);
	}
	return 0;
}

Mat smallFry(Mat& input, vector<int> colorQuants, int threshold) {
	for (int b = 0; b < 256; b++) {
		if (colorQuants[b] < threshold) {
			fixPixels(input, b, 0);
		}
	}
	return input;
}

vector<int> colorQuantities(Mat& input, vector<int> colors) {
	vector<int> quants;
	for (int b = 0; b < 256; b++) {
		quants[b] = 0;
		if (colors[b]) {
			for (int i = 0; i < input.rows; i++) {
				for (int j = 0; j < input.cols; j++) {
					uchar pixel = input.at<uchar>(i,j);
					if (pixel == b) {
						quants[b]++;
					}
				}
			}
		}
	}
	return quants;
}

vector<double> angles(Mat input, vector<int> colors) {
	vector<double> output;
	for (int b = 0; b < 256; b++) {
		output.push_back(0);
		if (colors[b]) {
			double theta = .5 * atan2(2 * centralMoment(input, b, 1, 1), (centralMoment(input, b, 2, 0) - centralMoment(input, b, 0, 2)));
			output[b] = theta;
		}
	}
	return output;
}

Mat rotateImage(Mat& source, double angle) {
    Point2f src_center(source.cols/2.0F, source.rows/2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, angle * 180 / M_PI, 1.0);
    Mat dst;
    warpAffine(source, dst, rot_mat, source.size());
    return dst;
}

vector<Mat> objects(Mat input, vector<double> degs, int& maxVal, vector<int> colors) {
	vector<Mat> objs;
	int limit = 0;
	for (int b = 0; b < 256; b++) {
		if (limit == maxVal) {
			break;
		}
		if (colors[b]) {
			int rMin = input.rows;
			int cMin = input.cols;
			int rMax = 0;
			int cMax = 0;
			for (int i = 0 ; i < input.rows; i++) {
				for (int j = 0; j < input.cols; j++) {
					uchar pixel = input.at<uchar>(i,j);
					if (pixel == b) {
						if (i < rMin) {
							rMin = i;
						} else if (i > rMax) {
							rMax = i;
						}
						if ( j < cMin) {
							cMin = j;
						} else if (j > cMax) {
							cMax = j;
						}
					}
				}
			}
			int width = (int)sqrt(pow(rMax - rMin, 2) + pow(cMax - cMin, 2));
			width = max(width, 150);
			int rows = rMax - rMin;
			int cols = cMax - cMin;
			Mat temp(width, width, CV_8UC1);
			for (int i = 0; i < temp.rows; i++) {
				for (int j = 0; j < temp.cols; j++) {
					temp.at<uchar>(i,j) = 0;
				}
			}
			for (int i = rMin; i < rMax + 1; i++) {
				for (int j = cMin; j < cMax + 1; j++) {
					uchar pixel = input.at<uchar>(i,j);
					int row = (width / 2) - (rows / 2) + (i - rMin);
					int col = (width / 2) - (cols / 2) + (j - cMin);
					if (pixel == b) {
						temp.at<uchar>(row, col) = pixel;
					}
				}
			}
			//temp = rotateImage(temp, degs[b]);
			objs.push_back(temp);
			limit++;
		}
	}
	if (limit != maxVal) {
		//limit has already been incremented at the end of the for loop; don't do it again
		maxVal = limit;
	}
	return objs;
}

Mat diff(Mat first, Mat second, int minLevel) {
	Mat third(second.rows, second.cols, CV_8UC1);
	for (int i = 0; i < third.rows; i++) {
		for (int j = 0; j < third.cols; j++) {
			Vec3b &firstPixels = first.at<Vec3b>(i,j);
			Vec3b &secondPixels = second.at<Vec3b>(i,j);
			uchar &outputPixels = third.at<uchar>(i,j);
			int average = abs(secondPixels[0] - firstPixels[0]) + abs(secondPixels[1] - firstPixels[1]) + abs(secondPixels[2] - firstPixels[2]);
			minLevel;
			average /= 3;
			if (average < minLevel) {
				outputPixels = 0;
			} else {
				outputPixels = 255;
			}
		}
	}
	return third;
}

Mat detectObjects(Mat input) {
	int obj = 25;
	int rows = input.rows;
	int cols = input.cols;
	Mat objects(rows, cols, CV_8UC1);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (input.at<uchar>(i,j) == 0) {
				objects.at<uchar>(i,j) = 0;
			} else {
				uchar left;
				uchar above = 0;
				bool setPixel = false;
				if (i > 0) {
					above = objects.at<uchar>(i-1,j);
					if (j > 0) {
						left = objects.at<uchar>(i,j-1);
						if (left != 0) {
							objects.at<uchar>(i,j) = left;
							setPixel = true;
							if (above != left && above != 0) {
								fixPixels(objects, left, above);
							}
						} 
					}
				} else {
					if (j > 0) {
						left = objects.at<uchar>(i,j-1);
						if (left > 0) {
							objects.at<uchar>(i,j) = left;
						} else {
							objects.at<uchar>(i,j) = obj;
							obj++;
						}
					} else {
						objects.at<uchar>(i,j) = obj;
						obj++;
					}
				}

				if (!setPixel) {
					if (above != 0) {
						objects.at<uchar>(i,j) = above;
					} else {
						objects.at<uchar>(i,j) = obj;
						obj++;
					}
				}
			}
		}
	}
	return objects;
}

Mat fixPixels(Mat& output, int oldVal, int newVal) {
	for (int i = 0; i < output.rows; i++) {
		for (int j = 0; j < output.cols; j++) {
			if (output.at<uchar>(i,j) == oldVal) {
				output.at<uchar>(i,j) = newVal;
			}
		}
	}
	return output;
}

int centralMoment(Mat input, int color, int p, int q) {
	int* centroidLoc = centroidColor(input, color);
	int totalVal = 0;
	for (int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			if (input.at<uchar>(i,j) == color) {
				int temp = (int)(i - centroidLoc[0], p) + (int)pow(j - centroidLoc[1], q);
				totalVal += temp;
			}
		}
	}
	return totalVal;
}

vector<int> colorsPresent(Mat output) {
	vector<int> colors;
	for (int i = 0; i < 256; i++) {
		colors.push_back(false);
	}
	for (int i = 0; i < output.rows; i++) {
		for (int j = 0; j < output.cols; j++) {
			uchar pixel = output.at<uchar>(i,j);
			if (pixel != 0) {
				colors[pixel] = true;
			}
		}
	}
	return colors;
}

int* centroidColor(Mat input, int color) {
	int rowAve = 0;
	int colAve = 0;
	int count = 0;
	for (int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			if (input.at<uchar>(i,j) == color) {
				rowAve += i;
				colAve += j;
				count++;
			}
		}
	}
	if (count > 0) {
		int width = 3;
		rowAve /= count;
		colAve /= count;
		int* returnVals = new int[2];
		returnVals[0] = rowAve;
		returnVals[1] = colAve;
		return returnVals;
	}
	return 0;
}

Mat centroid(Mat& input, vector<int> colors) {
	Mat cent(input.rows, input.cols, CV_8UC3);
	for (int b = 0; b < 256; b++) {
		if (colors[b]) {
			int rowAve = 0;
			int colAve = 0;
			int count = 0;
			for (int i = 0; i < cent.rows; i++) {
				for (int j = 0; j < cent.cols; j++) {
					uchar pixel = input.at<uchar>(i,j);
					cent.at<Vec3b>(i,j)[0] = pixel;
					cent.at<Vec3b>(i,j)[1] = pixel;
					cent.at<Vec3b>(i,j)[2] = pixel;
					if (pixel == b) {
						rowAve += i;
						colAve += j;
						count++;
					}
				}
			}
			rowAve /= count;
			colAve /= count;
			/*
			int theta = .5 * atan2(2 * centralMoment(input, b, 1, 1), (centralMoment(input, b, 2, 0) - centralMoment(input, b, 0, 2)));
			int row = rowAve - (10 * cos(theta));
			int col = colAve - (10 * sin(theta));
			centroid.at<Vec3b>(row, col) = *new Vec3b(0, 0, 255);
			row = rowAve + (10 * cos(theta));
			col = colAve + (10 * sin(theta));
			centroid.at<Vec3b>(row, col) = *new Vec3b(0, 0, 255);
			*/
			/*
			int w1 = 5;
			int w2 = 3;
			int row = rowAve - (w2 * sin(theta));
			int col = colAve + (w2 * cos(theta));
			centroid.at<Vec3b>(row, col)[0] = 0;
			centroid.at<Vec3b>(row, col)[1] = 0;
			centroid.at<Vec3b>(row, col)[2] = 255;
			row = rowAve + (w2 * cos(theta));
			col = colAve - (w2 * sin(theta));
			centroid.at<Vec3b>(row, col)[0] = 0;
			centroid.at<Vec3b>(row, col)[1] = 0;
			centroid.at<Vec3b>(row, col)[2] = 255;
			row = rowAve + (w1 * cos(theta));
			col = colAve - (w1 * sin(theta));
			centroid.at<Vec3b>(row, col)[0] = 0;
			centroid.at<Vec3b>(row, col)[1] = 0;
			centroid.at<Vec3b>(row, col)[2] = 255;
			row = rowAve - (w1 * cos(theta));
			col = colAve + (w1 * sin(theta));
			centroid.at<Vec3b>(row, col)[0] = 0;
			centroid.at<Vec3b>(row, col)[1] = 0;
			centroid.at<Vec3b>(row, col)[2] = 255;
			*/
			cent = drawX(cent, rowAve, colAve);
		}
	}
	return cent;
}

Mat drawX(Mat& input, int x, int y) {
	int width = 3;
	for (int i = max(0, x-width); i < min(x+width, input.rows-1); i++) {
		input.at<Vec3b>(x, y) = *new Vec3b(0, 0, 255);
	}
	for (int i = max(0, y-width); i < min(y+width, input.cols-1); i++) {
		input.at<Vec3b>(x, y) = *new Vec3b(0, 0, 255);
	}
	return input;
}