#include "D:\Solve-Puzzle-Not-Puzzled\main.h"
#include "ColorInfo.h"
#include <map>


void ImgDescriptor(Mat, int, int);
vector <vector<float>> ColorSort(vector<ColorInfo> origin, ColorInfo piece);
ColorInfo AnalysisColor(Mat img);
ColorInfo AnalysisColorPNG(Mat img);
vector <vector<float>> ColorAnalysis(Mat, Mat, int, int);

void ShowImageColorInfo(Mat img,vector<ColorInfo> color,int row,int col){

	int i=0;
	Mat part;
	vector<ColorInfo>::iterator it;
	
	for (it = color.begin(); it != color.end(); ++it)
	{
		
		//printf("%d-%d\n", i / col + 1, i%col+1);
		//it->ShowColorInfo(); 
		part = img(Range((int)((i / col)*(float)img.rows / row), (int)((float)img.rows / row*((i / col) + 1))), Range((int)((i%col)*(float)img.cols / col), (int)((float)img.cols / col*((i%col) + 1))));	//會越來越小
		//namedWindow("part", CV_WINDOW_AUTOSIZE);
		//imshow("part", part);
		//printf("\n\n");
		i++;
		//waitKey(1);
		i = i;
	}
	

}

ColorInfo AnalysisColorPNG(Mat img){
	int i, j, m;
	int N = 15;    /*class the color into N colors*/
	int h;

	vector<float> colorArray(N, 0);
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
			if (p[j][1] == 0 && p[j][2] == 0){
				//cout << "none";
				
			}
			else{
				colorArray[h]++;
			}
		}
	}
	for (i = 0; i < colorArray.size(); i++){
		colorArray[i] /= (img.rows*img.cols);
	}
	ColorInfo colorInfo(colorArray);
	colorInfo.ShowColorInfo();
	return colorInfo;

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

float ColorCompare(ColorInfo c1, ColorInfo c2){
	
	int i;
	int size = 15;
 	float compare = 0.0,a;
	
	for (i = 0; i < size; i++){
	
			a = abs(c1.get_colorProportion(i) - c2.get_colorProportion(i));

		compare = compare + a;
	}
	return compare;
	

}


ColorInfo PieceColorDescriptor(Mat img){

	Mat hsv;
	cvtColor(img, hsv, CV_BGR2HSV_FULL);

	ColorInfo pieceColor = AnalysisColor(hsv);
	pieceColor.ShowColorInfo();

	return pieceColor;

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

	return color;
	
}
bool compare_sortting(const vector<float> first, const vector<float> second)
{
	return first[1]<second[1];
}


vector <vector<float>> ColorCompareArray(vector<ColorInfo> origin, ColorInfo piece){


	vector <vector<float>> color_array;
	vector<float> colorInfo;
	int i;
	float compare;
	for (i = 0; i < origin.size(); i++){

		compare = ColorCompare(origin[i], piece);
		color_array.push_back(colorInfo);
		color_array[i].push_back((float)i);
		color_array[i].push_back(compare);

		cout << color_array[i][0] << "   " << color_array[i][1] << endl;

	}

	return color_array;
}

vector <vector<float>> ColorSort(vector <vector<float>> color_sort_array){

	int i;

	sort(color_sort_array.begin(), color_sort_array.end(), compare_sortting);

	for (i = 0; i < 27; i++)
	{
		cout << color_sort_array[i][0] << " : " << color_sort_array[i][1] << endl;
	}

	return color_sort_array;

}


vector <vector<float>> ColorAnalysis(Mat original, Mat piece, int row, int col){

	vector<ColorInfo> original_descriptor;
	vector <vector<float>> colorInfo;
	Mat piece_hsv;
	ColorInfo pieceColor;
	original_descriptor = ImgColorDescriptor(original, row, col);
	cvtColor(piece, piece_hsv, CV_BGR2HSV_FULL);
	pieceColor = AnalysisColorPNG(piece_hsv);
	colorInfo = ColorCompareArray(original_descriptor, pieceColor);
	//ColorSort(&colorInfo);
	return colorInfo;
}