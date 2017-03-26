#pragma once

#include<iostream>

#include<vector>

#include "math.h"

#include "stdafx.h"

#include "text.hpp"

#include "File.h"

#include <cv.h>

#include<map>

#include "highgui.h"

#include "Icone.h"

using namespace cv;



class ImageExtraction
{
	public:
		ImageExtraction();
		~ImageExtraction();

		std::vector<cv::Rect> detectLetters(cv::Mat img);
		std::vector<cv::Mat> prepareImage(Mat &img);
		std::vector<cv::Mat> extractionImages(cv::Mat &img);
		std::string matchingIcon(cv::Mat img);
		std::string matchingSize(cv::Mat img);

		double matchingTemplate(cv::Mat &img, cv::Mat &templ);



	private:
		std::map<string, Mat> icons;
		std::map<string, Mat> sizes;
};



