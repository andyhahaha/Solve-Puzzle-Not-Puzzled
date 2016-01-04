#include "main.h"

void draw_matches(Mat img1, Mat img2, vector< KeyPoint > keypoints1, vector< KeyPoint > keypoints2, vector< DMatch > matches, string number);
void symmetryTest(const vector<DMatch> matches1, const vector<DMatch> matches2, vector<DMatch> &symMatches);
vector< DMatch > get_good_dist_matches(vector< DMatch > matches, int min_scale);
vector<Mat> eachPart;
vector<Mat> totalDescriptors;
vector < vector < KeyPoint > > totalKeypoints;

vector<Mat> ImgSIFTDescriptor(Mat img, int row, int col)
{
	eachPart.clear();
	totalDescriptors.clear();
	totalKeypoints.clear();

	Mat part, img_gray;
	vector < KeyPoint > keypoints;
	SurfFeatureDetector detector;
	Mat descriptors;
	SurfDescriptorExtractor extractor;

	if (!img.data)//如果數據為空
	{
		cout << "Error: No image!" << endl;
		totalDescriptors.clear();
	}
	else
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				part = img(Range(round((float)img.rows / row*i), round((float)img.rows / row*(i + 1))), Range(round((float)img.cols / col*j), round((float)img.cols / col*(j + 1))));
				eachPart.push_back(part);
				cvtColor(part, img_gray, CV_RGB2GRAY);

				detector.detect(img_gray, keypoints);
				totalKeypoints.push_back(keypoints);
				extractor.compute(img_gray, keypoints, descriptors);
				totalDescriptors.push_back(descriptors);
				cout << "SIFT descriptors finish row = " << i << " col = " << j<< endl;
			}
		}
	}
	return totalDescriptors;
}



vector<float> SIFTAnalysis(Mat piece)
{
	vector<int> matchesAmount;
	vector<float> matchesPercent;
	int maxMatches = 0;
	int maxRow = 0, maxCol = 0, maxIndex = 0;

	Mat img_gray;
	vector < KeyPoint > pieceKeypoints;
	SurfFeatureDetector detector;
	Mat pieceDescriptors;
	SurfDescriptorExtractor extractor;
	FlannBasedMatcher matcher;
	vector<DMatch> matches1, matches2, good_matches;

	cvtColor(piece, img_gray, CV_RGB2GRAY);
	detector.detect(img_gray, pieceKeypoints);
	extractor.compute(img_gray, pieceKeypoints, pieceDescriptors);

	int i = 0;
	for (i = 0; i < totalDescriptors.size(); i++)
	{
		matcher.match(totalDescriptors[i], pieceDescriptors, matches1);
		matcher.match(totalDescriptors[i], pieceDescriptors, matches2);
		//draw_matches(eachPart[i], piece, totalKeypoints[i], pieceKeypoints, matches2, to_string(i) + "_1");

		//look for symmetry matches
		//vector< DMatch > sym_matches;
		//symmetryTest(matches1, matches2, sym_matches);
		//draw_matches(eachPart[i], piece, totalKeypoints[i], pieceKeypoints, sym_matches, to_string(i) + "_2");

		//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
		good_matches = get_good_dist_matches(matches2, 6);
		draw_matches(eachPart[i], piece, totalKeypoints[i], pieceKeypoints, good_matches, to_string(i) + "_1");

		matchesAmount.push_back(good_matches.size());
		if (good_matches.size()>maxMatches)
		{
			maxMatches = good_matches.size();
			maxIndex = i;
		}
	}

	for (i = 0; i < totalDescriptors.size(); i++)
	{
		matchesPercent.push_back((float)matchesAmount[i] / (float)maxMatches);
	}
	
	return matchesPercent;
}



void draw_matches(Mat img1, Mat img2, vector< KeyPoint > keypoints1, vector< KeyPoint > keypoints2, vector< DMatch > matches, string number)
{
	Mat img_matches;
	drawMatches(img1, keypoints1, img2, keypoints2,
		matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	imwrite("D:\\Solve-Puzzle-Not-Puzzled\\matches\\" + number + ".jpg", img_matches);
}


void symmetryTest(const vector<DMatch> matches1, const vector<DMatch> matches2, vector<DMatch> &symMatches)
{
	//matches1是img1->img2，matches2是img2->img1
	symMatches.clear();
	for (vector<DMatch>::const_iterator matchIterator1 = matches1.begin(); matchIterator1 != matches1.end(); ++matchIterator1)
	{
		for (vector<DMatch>::const_iterator matchIterator2 = matches2.begin(); matchIterator2 != matches2.end(); ++matchIterator2)
		{
			if ((*matchIterator1).queryIdx == (*matchIterator2).trainIdx && (*matchIterator2).queryIdx == (*matchIterator1).trainIdx)
			{
				symMatches.push_back(DMatch((*matchIterator1).queryIdx, (*matchIterator1).trainIdx, (*matchIterator1).distance));
				break;
			}
		}
	}
}


vector< DMatch > get_good_dist_matches(vector< DMatch > matches, int min_scale)
{
	double max_dist = 0;
	double min_dist = 100;
	double distance;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < matches.size(); i++)
	{
		distance = matches[i].distance;
		if (distance < min_dist) min_dist = distance;
		if (distance > max_dist) max_dist = distance;
	}

	//-- Use only "good" matches (i.e. whose distance is less than min_scale*min_dist )
	vector< DMatch > good_matches;

	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance < min_scale * min_dist)
			good_matches.push_back(matches[i]);
	}
	return good_matches;
}