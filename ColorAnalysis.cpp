#include "D:\Solve-Puzzle-Not-Puzzled\main.h"
#include "ColorInfo.h"



void ImgDescriptor(Mat, int, int);
ColorInfo AnalysisColor(Mat img){
	int i, j;
	/*for (i = 0; i < img.rows; i++){
		for (j = 0; j < img.cols; j++){

		}
	}*/
	ColorInfo color;
	MatIterator_<Vec3b>imgit, imgend;
	for (imgit = img.begin<Vec3b>(), imgend = img.end<Vec3b>(); imgit != imgend; ++imgit){
		cout << (*imgit)[0] << endl;

	}
	return color;
}


void ImgDescriptor(Mat img,int row,int col){

	Mat hsv;
	vector<Mat> hsv_split, part_split;
	Mat part;
	ColorInfo color;
	int i, j;
	int width, height;
	width = hsv.cols / col;
	height = hsv.rows / row;
	cvtColor(img, hsv, CV_BGR2HSV_FULL);

	cout << hsv.type() << endl;
	/*for (i = 0; i < row; i++){
		for (j = 0; j < col; j++){
			
			part = hsv(Range(hsv.rows / row*i, hsv.rows / row*(i + 1)), Range(hsv.cols / col*j, hsv.cols / col*(j + 1)));

		}
	}*/
	i = 0;
	j = 0;
	part = img(Range(0, hsv.rows / row), Range(0, hsv.cols / col));
	
	



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

	for (i = 0; i < part_split.at(0).rows; ++i){

		uchar *p = part_split.at(0).ptr<uchar>(i);
		for (j = 0; j < part_split.at(0).cols; ++j){
			cout << p[j] << endl;

		}

	}
}