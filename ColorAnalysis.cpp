#include "D:\Solve-Puzzle-Not-Puzzled\main.h"
#include "ColorInfo.h"
#include <map>


void ImgDescriptor(Mat, int, int);


void ShowImageColorInfo(Mat img,vector<ColorInfo> color,int row,int col){

	int i=0;
	Mat part;
	vector<ColorInfo>::iterator it;
	for (it = color.begin(); it != color.end(); ++it)
	{
		
		printf("%d-%d\n", i / col + 1, i%col+1);
		it->ShowColorInfo(); 
		part = img(Range((i / col)*img.rows / row, img.rows / row*((i / col) + 1)), Range((i%col)*img.cols / col, img.cols / col*((i%col) + 1)));	//會越來越小
		namedWindow("part", CV_WINDOW_AUTOSIZE);
		imshow("part", part);
		printf("\n\n");
		i++;
		waitKey(1);
		i = i;
	}
}

ColorInfo AnalysisColor(Mat img){
	int i, j,m;
	int N = 15;    /*class the color into N colors*/
	int h;
	
	vector<float> colorArray(N,0);
	map<int, float> colorPrortion;
	for (i = 0; i < img.rows; ++i){

		Vec3b *p = img.ptr<Vec3b>(i);
		for (j = 0; j < img.cols; ++j){
			if (p[j][1] < COLOR_RANGE / 8){
				if (p[j][2] < COLOR_RANGE / 3){				//black
					h = 14;
				}
				else if (p[j][2] > 2 * COLOR_RANGE / 3){	//white
					h = 12;
				}
				else{										//gray
					h = 13;
				}

			}
			else{
				h = (float)p[j][0] / COLOR_RANGE * N;

			}
		
			colorArray[h]++;
		}
	}
	for (i = 0; i < colorArray.size(); i++){
		colorArray[i] /= (img.rows*img.cols);
	}
	ColorInfo colorInfo(colorArray);
	return colorInfo;
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



vector<ColorInfo> ImgColorDescriptor(Mat img, int row, int col){

	Mat hsv;
	vector<Mat> hsv_split, part_split;
	Mat part;
	vector<ColorInfo> color;
	int i, j;
	int width, height;
	width = hsv.cols / col;
	height = hsv.rows / row;
	cvtColor(img, hsv, CV_BGR2HSV_FULL);

	//cout << hsv.type() << endl;
	for (i = 0; i < row; i++){
		for (j = 0; j < col; j++){
			
			part = hsv(Range(hsv.rows / row*i, hsv.rows / row*(i + 1)), Range(hsv.cols / col*j, hsv.cols / col*(j + 1)));
			color.push_back(AnalysisColor(part));
		}
	}

	ShowImageColorInfo(img,color,row,col);



	/*split(hsv, hsv_split);
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
	*/
	//PrintColorImage(part, 0);
	AnalysisColor(part);

	return color;
	
}