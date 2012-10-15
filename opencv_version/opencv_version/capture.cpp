
// Video Image PSNR and SSIM
#include "stdafx.h"

#include <iostream>	// for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion

#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O

using namespace std;
using namespace cv;
// A Simple Camera Capture Framework 
int main() {
CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
if ( !capture ) {
    fprintf( stderr, "ERROR: capture is NULL \n" );
    getchar();
    return -1;
}
// Create a window in which the captured images will be presented
cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
// Show the image captured from the camera in the window and repeat
while ( 1 ) {
    // Get one frame
    IplImage* frame = cvQueryFrame( capture );
    if ( !frame ) {
    fprintf( stderr, "ERROR: frame is null...\n" );
    getchar();
    break;
    }
    cvShowImage( "mywindow", frame );
    // Do not release the frame!
    //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
    //remove higher bits using AND operator
    if ( (cvWaitKey(10) & 255) == 27 ) break;
}
// Release the capture device housekeeping
cvReleaseCapture( &capture );
cvDestroyWindow( "mywindow" );
return 0;
}