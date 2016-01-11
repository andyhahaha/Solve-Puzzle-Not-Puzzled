#include "D:\Solve-Puzzle-Not-Puzzled\main.h"


Mat LoadImg(string path){
	Mat img;
	img = imread(path);
	return img;
}


bool sortCompare(const vector<float> &first, const vector<float> &second)
{
	return first[1]<second[1];
}

vector <vector<float>> Sort(vector <vector<float>> sort_array)
{
	sort(sort_array.begin(), sort_array.end(), sortCompare);
	return sort_array;
}

vector< vector<float> >MergeResult(vector< vector<float>> sift, vector< vector<float>> color)
{
	int i;
	vector<float> colorInfo;
	vector< vector<float>> merge, mergeSorted;
	for (i = 0; i < sift.size(); i++)
	{
		merge.push_back(colorInfo);
		merge[i].push_back(sift[i][0]);
		if (sift[i][1]==100)
			merge[i].push_back( color[i][1]);
		else
			merge[i].push_back(sift[i][1] * color[i][1]);
	}
	return Sort(merge);
}

void drawRect(Mat img, int index, int sort)
{
	Mat rect = img(Rect(100, 100, 300, 300));
	Mat color(rect.size(), CV_8UC3, cv::Scalar(0, 125, 125));
	double alpha = 0.3;
	addWeighted(color, alpha, rect, 1.0 - alpha, 0.0, rect);
}

void ShowResult(vector< vector<float>> result,Mat img,int row,int col)
{
	int i=0,sort,offset=10;
	Mat rect;
	double alpha = 0.3;
	double width = img.cols / (double)col, height = img.rows / (double)row;
	cout << "FINAL RESULT" << endl << endl;

	rect = img(Rect(i / col*height, i%col*width, i / col*height + height, i%col*width + width));
	Mat color1(rect.size(), CV_8UC3, cv::Scalar(255, 0, 0));
	Mat color2(rect.size(), CV_8UC3, cv::Scalar(255, 157, 0));
	Mat color3(rect.size(), CV_8UC3, cv::Scalar(255, 247, 0));
	Mat color4(rect.size(), CV_8UC3, cv::Scalar(59, 255, 0));
	Mat color5(rect.size(), CV_8UC3, cv::Scalar(0, 255, 187));
	Mat color6(rect.size(), CV_8UC3, cv::Scalar(0, 242, 255));
	Mat color7(rect.size(), CV_8UC3, cv::Scalar(0, 68, 255));
	Mat color8(rect.size(), CV_8UC3, cv::Scalar(179, 0, 255));
	Mat color9(rect.size(), CV_8UC3, cv::Scalar(255, 0, 221));

	int poinit1y, poinit1x, poinit2y, poinit2x;

	for (i = 0; i < result.size(); i++){
		cout << i+1<<"  "<<result[i][0] << "  " << result[i][1] << endl;

		//rect = img(Rect((int)((float)((int)result[i][0] / col)*height), (int)((float)((int)result[i][0] % col)*width), (int)((float)((int)result[i][0] / col)*height) + (int)height, (int)((float)((int)result[i][0] % col)*width) + (int)width));
		sort = (int)result[i][0];
		poinit1y = sort / col*height;
		poinit1x = sort % col*width;
		poinit2y = sort / col*height + height - 1;
		poinit2x = sort % col*width + width - 1;
		//Mat rectan = img(Rect(poinit1x, poinit1y, width, height));
		Point point1(poinit1x + offset, poinit1y + offset);
		Point point2(poinit2x - offset, poinit2y - offset);

		//rectangle(img, point1, point2, Scalar(0, 255, 0),2);	//B G R
		/*if (i<col*row/9)
			rectangle(img, point1, point2, Scalar(0, 0, 255), 5);
		else if (i<col*row / 9 * 2)
			rectangle(img, point1, point2, Scalar(0, 157, 255), 5);
		else if (i<col*row / 9 * 3)
			rectangle(img, point1, point2, Scalar(0, 247, 255), 5);
		else if (i<col*row / 9 * 4)
			rectangle(img, point1, point2, Scalar(0, 255, 59), 5);
		else if (i<col*row / 9 * 5)
			rectangle(img, point1, point2, Scalar(187, 255, 0), 5);
		else if (i<col*row / 9 * 6)
			rectangle(img, point1, point2, Scalar(255, 242, 0), 5);
		else if (i<col*row / 9 * 7)
			rectangle(img, point1, point2, Scalar(255, 68, 0), 5);
		else if (i<col*row / 9 * 8)
			rectangle(img, point1, point2, Scalar(255, 0, 179), 5);
		else if (i<col*row / 9 * 9)
			rectangle(img, point1, point2, Scalar(211, 0, 255), 5);
			*/
		if (i == 0)
			rectangle(img, point1, point2, Scalar(0, 0, 255), 5);
		else if (i<4)
			rectangle(img, point1, point2, Scalar(255, 242, 0), 5);
		else if (i<10)
			rectangle(img, point1, point2, Scalar(0, 255, 59), 5);
	}

	Mat img_normal;
	resize(img, img_normal, Size(img.cols *0.7, img.rows *0.7));
	namedWindow("結果", CV_WINDOW_AUTOSIZE);
	imshow("結果", img_normal);

	waitKey(100000000);
}



void main()
{
	string img_path,piece_path;
	Mat original, piece, result;
	vector <vector<float>> SIFT_result, Color_result, Color_result6, Color_result6_sort, Merge_result;
	int number,i;
	int row = 6, col = 9;	/*luck apple:24x42 pieces     fox:6x9 pieces */
	
	//prepare original image
	img_path = "D:\\Solve-Puzzle-Not-Puzzled\\resource\\fox.jpg";
	original = LoadImg(img_path);

	//analysis original image
	ImgSIFTDescriptor(original, row, col);

	while (1)
	{
		original = LoadImg(img_path);
		namedWindow("原圖", CV_WINDOW_AUTOSIZE);
		imshow("原圖", original);
		waitKey(1);
		cout << "please type piece number : ";
		cin >> number;
		cout << number << endl;

		//prepare piece image
		piece_path = "D:\\Solve-Puzzle-Not-Puzzled\\resource\\foxPieces\\fox" + to_string(number) + ".png";
		piece = LoadImg(piece_path);
		cout << "find " << piece_path << endl;
		namedWindow("拼圖", CV_WINDOW_AUTOSIZE);
		imshow("拼圖", piece);
		waitKey(1);

		//color analysis
		ImgColorDescriptor(original, row, col, 9);
		Color_result6 = ColorAnalysis(original, piece, row, col, 9);
		Color_result6_sort = Sort(Color_result6);
		/*for (i = 0; i < Color_result6.size(); i++){
			cout << "6 color  " << i + 1 << "  " << Color_result6_sort[i][0] << "   " << Color_result6_sort[i][1] << endl;

		}*/
		//ImgColorDescriptor(original, row, col, 15);
		//Color_result = ColorAnalysis(original, piece, row, col,15);
	
		//SIFT analysis
		SIFT_result = SIFTAnalysis(piece, Color_result6_sort);

		Merge_result = MergeResult(SIFT_result, Color_result6);
		ShowResult(Merge_result,original,row,col);
	}
}