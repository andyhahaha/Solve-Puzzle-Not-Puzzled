#include "D:\Solve-Puzzle-Not-Puzzled\main.h"
#include "ColorInfo.h"



void ImgDescriptor(Mat, int, int);
ColorInfo AnalysisColor(Mat img){
	int i, j;
	int h;
	ColorInfo color;
	for (i = 0; i < img.rows; ++i){

		Vec3b *p = img.ptr<Vec3b>(i);
		for (j = 0; j < img.cols; ++j){
			h = (float)p[j][0] / 255 * 6;

		}
		printf("\n");
	}
	return color;
}

void PrintColorImage(Mat img, int channel){

	int i, j;
	int h;

	for (i = 0; i < img.rows; ++i){

		Vec3b *p = img.ptr<Vec3b>(i);
		for (j = 0; j < img.cols; ++j){
			h = (float)p[j][channel]/255*6;
			printf("%d ", h);
		}
		printf("\n");
	}
}

void PrintNonColorImage(Mat img){

	int i, j;
	for (i = 0; i < img.rows; ++i){

		uchar *p = img.ptr<uchar>(i);
		for (j = 0; j < img.cols; ++j){
			printf("%3d ", p[j]);
		}
		printf("\n");
	}
}



void ImgColorDescriptor(Mat img, int row, int col){

	Mat hsv;
	vector<Mat> hsv_split, part_split;
	Mat part;
	ColorInfo color;
	int i, j;
	int width, height;
	width = hsv.cols / col;
	height = hsv.rows / row;
	cvtColor(img, hsv, CV_BGR2HSV_FULL);

	//cout << hsv.type() << endl;
	/*for (i = 0; i < row; i++){
		for (j = 0; j < col; j++){
			
			part = hsv(Range(hsv.rows / row*i, hsv.rows / row*(i + 1)), Range(hsv.cols / col*j, hsv.cols / col*(j + 1)));

		}
	}*/
	i = 4;
	j = 5;
	part = img(Range(hsv.rows / row*i, hsv.rows / row*(i+1)), Range(hsv.cols / col*j, hsv.cols / col*(j+1)));
	
	



	split(hsv, hsv_split);
	split(part, part_split);
	namedWindow("原圖", CV_WINDOW_AUTOSIZE);
	namedWindow("HSV", CV_WINDOW_AUTOSIZE);
	namedWindow("part", CV_WINDOW_AUTOSIZE);
	imshow("原圖",img);
	imshow("HSV",hsv);
	imshow("part", part);
	namedWindow("hue", CV_WINDOW_AUTOSIZE);
	namedWindow("saturation", CV_WINDOW_AUTOSIZE);
	namedWindow("value", CV_WINDOW_AUTOSIZE);
	imshow("hue", hsv_split.at(0));
	imshow("saturation", hsv_split.at(1));
	imshow("value", hsv_split.at(2));

	PrintColorImage(part, 0);
	
	
}