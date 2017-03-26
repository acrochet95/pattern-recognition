#pragma once

#include<iostream>
#include <fstream>
#include<vector>
#include "math.h"
#include "stdafx.h"

#include "text.hpp"

#include <cv.h>

#include<map>

#include "highgui.h"
#include "Icone.h"
#include "WriteARFF.h"
#include<Windows.h>

using namespace cv;

class FeaturesExtraction
{
	public:
		FeaturesExtraction();
		~FeaturesExtraction();

		void start();

		void caract(cv::Mat &img, Icone &icone);
		float getRatio(cv::Mat &img);
		vector<pair<int, int> > zoning(int size, cv::Mat& canny_img);
		cv::Point getCentroid(cv::Mat img);

	private :
		vector<string> files;
};

