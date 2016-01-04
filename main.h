#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <string>
#include <cstring>

#include <opencv2/contrib/contrib.hpp> 
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/nonfree/features2d.hpp>

using namespace std;
using namespace cv;

extern Mat LoadOriginalImg(string);

extern Mat LoadPieceImg(string);

extern vector<float>SIFTAnalysis(Mat);

extern vector<float>ColorAnalysis(Mat,Mat);

extern vector<float>MergeResult(vector<float>, vector<float>);

extern void ShowResult(vector<float>);

extern vector<Mat> ImgSIFTDescriptor(Mat, int, int);

extern void ImgColorDescriptor(Mat, int, int);
#endif