#include "main.h"

void draw_matches(Mat img1, Mat img2, vector< KeyPoint > keypoints1, vector< KeyPoint > keypoints2, vector< DMatch > matches, string number);
void symmetryTest(const vector<DMatch> matches1, const vector<DMatch> matches2, vector<DMatch> &symMatches);
float get_good_dist_matches(vector< DMatch > matches, vector< DMatch > &good_matches, float min_scale);
vector<Mat> eachPart;
vector<Mat> totalDescriptors;
vector < vector < KeyPoint > > totalKeypoints;

void ImgSIFTDescriptor(Mat img, int row, int col)
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
				//cout << "SIFT descriptors finish row = " << i << " col = " << j<< endl;
			}
		}
		cout << "SIFT descriptors finish. "<< endl;
	}
}



vector < vector<float>> SIFTAnalysis(Mat piece, vector <vector<float>> colorResult)
{
	vector < vector<float>> matchesAvgDist, matchesAvgDistSorted;
	float minDist = 100.0, maxDist = 0.0;
	float averageDist;

	matchesAvgDist.clear();
	matchesAvgDistSorted.clear();
	matchesAvgDist.resize(totalDescriptors.size());
	matchesAvgDistSorted.resize(totalDescriptors.size());

	Mat img_gray;
	vector < KeyPoint > pieceKeypoints;
	SurfFeatureDetector detector;
	Mat pieceDescriptors;
	SurfDescriptorExtractor extractor;
	FlannBasedMatcher matcher;
	vector<DMatch> matches1, matches2, good_matches, sym_matches;

	cvtColor(piece, img_gray, CV_RGB2GRAY);
	detector.detect(img_gray, pieceKeypoints);
	extractor.compute(img_gray, pieceKeypoints, pieceDescriptors);

	int i = 0, index = 0;
	for (i = 0; i < colorResult.size(); i++)
	{
		index = colorResult[i][0];
		averageDist = 0.0;
		if (i <= colorResult.size() / 2 && totalDescriptors[index].rows > 0)
		{
			matcher.match(pieceDescriptors, totalDescriptors[index], matches1);
			averageDist = get_good_dist_matches(matches1, good_matches, 3);
		}
		else
		{
			good_matches.clear();
			averageDist = 100;
		}
		matchesAvgDist[index].push_back((float)index);
		matchesAvgDist[index].push_back(averageDist);
		matchesAvgDist[index].push_back(good_matches.size());

		if (averageDist > maxDist && averageDist != 100)
			maxDist = averageDist;
		if (averageDist < minDist)
			minDist = averageDist;
	}

	for (i = 0; i < matchesAvgDist.size(); i++)
	{
		if (matchesAvgDist[i][1] != 100)
			matchesAvgDist[i][1] = (matchesAvgDist[i][1] - minDist)*0.99 / (maxDist - minDist) + 0.01;
	}

	/*matchesAvgDistSorted.assign(matchesAvgDist.begin(), matchesAvgDist.end());
	sort(matchesAvgDistSorted.begin(), matchesAvgDistSorted.end(), sortCompare);
	for (i = 0; i < matchesAvgDistSorted.size(); i++)
	{
		cout << "matchesAvgDistSorted " << i << " : " << matchesAvgDistSorted[i][0] << "  " << matchesAvgDistSorted[i][1] << "  " << matchesAvgDistSorted[i][2] << endl;
	}*/
	return matchesAvgDist;
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


float get_good_dist_matches(vector< DMatch > matches, vector< DMatch > &good_matches, float min_scale)
{
	double max_dist = 0;
	double min_dist = 100;
	double distance;
	float average_dist = 0.0;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < matches.size(); i++)
	{
		distance = matches[i].distance;
		if (distance < min_dist) min_dist = distance;
		if (distance > max_dist) max_dist = distance;
	}

	//-- Use only "good" matches (i.e. whose distance is less than min_scale*min_dist )
	good_matches.clear();
	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance < min_dist* min_scale)
		{
			good_matches.push_back(matches[i]);
			average_dist += matches[i].distance;
		}	
	}
	average_dist /= (float)matches.size();
	return average_dist;
}



void SIFTAnalysis2(Mat original, Mat piece)
{
	Mat original_gray, piece_gray;
	vector < KeyPoint > originalKeypoints, pieceKeypoints;
	SurfFeatureDetector detector;
	Mat originalDescriptors, pieceDescriptors;
	SurfDescriptorExtractor extractor;
	FlannBasedMatcher matcher;
	vector<DMatch> matches1, matches2, symmetryMatches, goodMatches;

	cvtColor(original, original_gray, CV_RGB2GRAY);
	cvtColor(piece, piece_gray, CV_RGB2GRAY);
	detector.detect(original_gray, originalKeypoints);
	detector.detect(piece_gray, pieceKeypoints);
	extractor.compute(original_gray, originalKeypoints, originalDescriptors);
	extractor.compute(piece_gray, pieceKeypoints, pieceDescriptors);
	matcher.match(pieceDescriptors, originalDescriptors, matches1);
	matcher.match(originalDescriptors, pieceDescriptors, matches2);

	symmetryTest(matches1, matches2, symmetryMatches);

	get_good_dist_matches(symmetryMatches, goodMatches, 3);
	draw_matches(piece, original, pieceKeypoints, originalKeypoints, goodMatches, "pieceToPart1");
	//draw_matches(original, piece, originalKeypoints, pieceKeypoints, goodMatches, "pieceToPart1");
}