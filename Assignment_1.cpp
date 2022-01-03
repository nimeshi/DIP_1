#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	Mat img = imread(argv[1], 1);

	if (!img.data) {
		cout << "Could not find the image!" << endl;
		return -1;
	}

	int height = img.rows;
	int width = img.cols;
	int widthstep = img.step;
	int ch = img.channels();

	
	Mat gray(height, width, CV_8UC1,Scalar(0));
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Mat new_image = gray.clone();

	int v;
	int output;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			v =(int)gray.at<uchar>(y, x);
		if ((v >= 0) && (v <= 72) ){
				output = int((5 / 72.0)*v);
			}
			else if ((v > 72) && (v <= 192)) {
				output = int((245 / 120.0)*(v - 72) + 5);
			}
			else if ((v > 192) && (v <= 255)) {
				output = int((5 / 63)*(v - 192) + 250);
			}
			
			new_image.at<uchar>(y, x) = (uchar)output;
		}
	}

	//allocate memory for all the intensity values
	int hist[256];

	//initialize "0" to all memory elememnt
	for (int i = 0; i < 256; i++) {
		hist[i] = 0;
	}
	

	//Count one by one pixel for each intensity values
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < height; x++) {
			hist[(int)new_image.at<uchar>(y, x)] = hist[(int)new_image.at<uchar>(y, x)] + 1;

		}
	}

	for (int i = 0; i < 256; i++) {
		cout << i << ":" << hist[i] << endl;
	}

	//create an image to display the histogram
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0));
	
	//Find the maximum no of pixels from histogram array
	int max = hist[0];
	for (int i = 0; i < 256; i++) {
		if (max < hist[i]) {
			max = hist[i];
		}
	}
	//Normalize the histogram between 0 and histogram h

	for (int i = 0; i < 256; i++) {
		hist[i] = ((double)hist[i] / max)*histImage.rows;
	}
	//Draw the lines of histogram
	for (int i = 0; i < 256; i++) {
		line(histImage, Point(bin_w*(i), hist_h), Point(bin_w*(i), hist_h - hist[i]), Scalar(255), 1, 8, 0);

		
	}

	namedWindow("Image"); //create window
	imshow("Image", img);
	namedWindow("Gray"); //create window
	imshow("Gray", gray);
	namedWindow("New Image"); //create window
	imshow("New Image", new_image);
	namedWindow("Histogram"); //create window
	imshow("Histogram", histImage);

	
	waitKey(0);
	return 0;
}