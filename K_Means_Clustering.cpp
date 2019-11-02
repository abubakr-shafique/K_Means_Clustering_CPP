#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat K_Means(Mat Input, int K);

int main() {

	Mat Input_Image = imread("Test_Image.png");
	cout << "Height: " << Input_Image.rows << ", Width: " << Input_Image.rows << ", Channels: " << Input_Image.channels() << endl;

	int Clusters = 8;
	Mat Clustered_Image = K_Means(Input_Image, Clusters);

	imwrite("Clustered_Image.png", Clustered_Image);
	system("pause");

	return 0;
}

Mat K_Means(Mat Input, int K) {
	Mat samples(Input.rows * Input.cols, Input.channels(), CV_32F);
	for (int y = 0; y < Input.rows; y++)
		for (int x = 0; x < Input.cols; x++)
			for (int z = 0; z < Input.channels(); z++)
				if (Input.channels() == 3) {
					samples.at<float>(y + x * Input.rows, z) = Input.at<Vec3b>(y, x)[z];
				}
				else {
					samples.at<float>(y + x * Input.rows, z) = Input.at<uchar>(y, x);
				}

	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, K, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);


	Mat new_image(Input.size(), Input.type());
	for (int y = 0; y < Input.rows; y++)
		for (int x = 0; x < Input.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * Input.rows, 0);
			if (Input.channels()==3) {
				for (int i = 0; i < Input.channels(); i++) {
					new_image.at<Vec3b>(y, x)[i] = centers.at<float>(cluster_idx, i);
				}
			}
			else {
				new_image.at<uchar>(y, x) = centers.at<float>(cluster_idx, 0);
			}
		}
	//imshow("clustered image", new_image);
	return new_image;
}