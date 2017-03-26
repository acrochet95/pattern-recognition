#include "stdafx.h"
#include "ImageExtraction.h"

ImageExtraction::ImageExtraction()
{
	icons["accident"] = imread("..\\accident.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["bomb"] = imread("..\\bomb.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["car"] = imread("..\\car.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["casualty"] = imread("..\\casualty.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["electricity"] = imread("..\\electricity.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["fire"] = imread("..\\fire.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["fire_brigade"] = imread("..\\fire_brigade.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["flood"] = imread("..\\flood.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["gas"] = imread("..\\gas.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["injury"] = imread("..\\injury.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["paramedics"] = imread("..\\paramedics.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["person"] = imread("..\\person.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["police"] = imread("..\\police.png", CV_LOAD_IMAGE_GRAYSCALE);
	icons["road_block"] = imread("..\\road_block.png", CV_LOAD_IMAGE_GRAYSCALE);

	//for (auto it = icons.begin(); it != icons.end(); ++it)
	//	threshold(it->second, it->second, 170, 255, THRESH_BINARY);

	sizes["small"] = imread("..\\small.png", CV_LOAD_IMAGE_GRAYSCALE);
	sizes["medium"] = imread("..\\medium.png", CV_LOAD_IMAGE_GRAYSCALE);
	sizes["large"] = imread("..\\large.png", CV_LOAD_IMAGE_GRAYSCALE);

	//for (int i = 0; i < sizes.size(); i++)
	//	threshold(sizes[i], sizes[i], 170, 255, THRESH_BINARY);

	srand(time(NULL));
}

ImageExtraction::~ImageExtraction()
{
	icons.clear();
	sizes.clear();
}

/*

TO DO

- text region detection
- text cropping (split the text region into several characters)
- then apply OCR algorithm.

*/


/*
http://stackoverflow.com/questions/23506105/extracting-text-opencv
*/


/* WE DON'T USE IT YET */
std::vector<cv::Rect> ImageExtraction::detectLetters(cv::Mat img)

{

	std::vector<cv::Rect> boundRect;

	cv::Mat img_gray, img_sobel, img_threshold, element;



	cvtColor(img, img_gray, CV_BGR2GRAY);

	cv::Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
	//cv::imshow("Sobel", img_sobel);

	cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	//cv::imshow("Seuil", img_threshold);

	element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3));


	cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element);
	//cv::imshow("Seuil2", img_threshold);

	std::vector< std::vector< cv::Point> > contours;

	cv::resize(img_threshold, img_threshold, cv::Size(707, 1000));



	cv::findContours(img_threshold, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	std::vector<std::vector<cv::Point> > contours_poly(contours.size());

	std::cout << contours.size() << std::endl;

	float nb_c = 7;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > 20)
		{

			cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 0, true);
			cv::Rect appRect(boundingRect(cv::Mat(contours_poly[i])));
			if (appRect.width > appRect.height)
			{
				/*float width = (float)appRect.width / nb_c ;
				std::cout << width << std::endl;
				for (int j = 0; j < nb_c; j++)
				{
					boundRect.push_back(cv::Rect(appRect.x-1 + j*(width), appRect.y, width, appRect.height));
				}*/

				boundRect.push_back(appRect);				
			}
		}
	}

	return boundRect;

}

std::vector<cv::Mat> ImageExtraction::prepareImage(Mat &img)
{
	std::vector<cv::Mat> values;

	Mat img_display(img, Rect(0, 650, img.size().width, 2500));

	const int height = img_display.size().height / 7;
	for (int i = 0; i < 7; i++)
	{
		Mat im(img_display, Rect(0, i*(height), img_display.size().width, height));
		values.push_back(im);
	}

	return values;
}

std::vector<cv::Mat> ImageExtraction::extractionImages(cv::Mat &img)
{
	std::vector<cv::Mat> boundImage;
	std::vector<cv::Rect> boundRect;

	cv::Mat img_gray, bw, img_sobel, img_threshold, element;


	cvtColor(img, img_gray, CV_BGR2GRAY);

	cv::Canny(img_gray, bw, 80, 240, 3);

	std::vector< std::vector< cv::Point> > contours;

	cv::findContours(bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	std::vector<std::vector<cv::Point> > contours_poly(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > 200)
		{
			// Approximate contour with accuracy proportional
			// to the contour perimeter
			cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 0, true); 

			cv::Rect appRect(boundingRect(cv::Mat(contours_poly[i])));

			// Filtrage
			const int SIZE = 260;
			if (appRect.width > SIZE - 10 && appRect.width < SIZE + 10 && appRect.height > SIZE - 10 && appRect.height < SIZE + 10)
			{
				// Rognage pour enlever les bords
				const int rognage = 8;
				appRect.x += rognage;
				appRect.y += rognage;
				appRect.width -= rognage*2;
				appRect.height -= rognage*2;

				bool oneIsSame = false;
				for (int j = 0; j < boundRect.size(); j++)
				{
					if (sqrt(pow((boundRect[j].x - appRect.x), 2) + pow(boundRect[j].y - appRect.y, 2)) < 15)
					{
						oneIsSame = true;
						break;
					}
				}

				// If there is no doublon -> add
				if (!oneIsSame){
					boundRect.push_back(appRect);

					cv::Mat croppedFaceImage(bw, appRect);

					std::vector< std::vector< cv::Point> > contours2;
					cv::findContours(croppedFaceImage, contours2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

					std::vector<std::vector<cv::Point> > contours_poly2(contours2.size());

					float max = 0;
					cv::Rect bound;

					float x(100000), y(100000), x2(0), y2(0);
					for (int j = 0; j < contours2.size(); j++)
					{

						cv::approxPolyDP(cv::Mat(contours2[j]), contours_poly2[j], 0, true);

						cv::Rect appRect2(boundingRect(cv::Mat(contours_poly2[j])));

						if (appRect2.x < x)
							x = appRect2.x;
						if (appRect2.y < y)
							y = appRect2.y;
						if (appRect2.x+appRect2.width > x2)
							x2 = appRect2.x + appRect2.width;
						if (appRect2.y + appRect2.height > y2)
							y2 = appRect2.y + appRect2.height;

					}

					if (x != 100000 && y != 100000)
					{
						cv::Mat empty_image(SIZE, SIZE, CV_8UC3, Scalar(255, 255, 255));

						float posx(0), posy(-1);
						float scale = SIZE / (x2 - x);
						if ((y2 - y) * scale > SIZE)
						{
							scale = SIZE / (y2 - y);
							posy = 0;
						}

						cv::Mat symbol(img, cv::Rect(appRect.x + x, appRect.y + y, x2 - x, y2 - y));

						cv::resize(symbol, symbol, cvSize(0, 0), scale, scale);

						if (posy == 0)
							posx = empty_image.size().width / 2 - symbol.size().width / 2;
						else
							posy = empty_image.size().height / 2 - symbol.size().height / 2;

						symbol.copyTo(empty_image(cv::Rect(posx, posy, symbol.cols, symbol.rows)));

						boundImage.push_back(empty_image);
					}
				}

			}
		}
	}



	return boundImage;
}


std::string ImageExtraction::matchingIcon(cv::Mat img)
{
	cvtColor(img, img, CV_BGR2GRAY);

	std::vector<std::string> elements;


	string value = "";

	std::map<string, double> val;
	for (auto it = icons.begin(); it != icons.end(); ++it)
	{
		val[it->first] = this->matchingTemplate(img, it->second);
	}

		// Select the min
	double min = val.begin()->second;
	value = val.begin()->first;
	for (auto it = val.begin(); it != val.end(); ++it)
	{
		if (it->second < min && it->second != 0)
		{
			min = it->second;
			value = it->first;
		}
	}

	
	return value;
}

std::string ImageExtraction::matchingSize(cv::Mat img)
{

	cvtColor(img, img, CV_BGR2GRAY);

	std::vector<std::string> elements;


	string value = "";

	std::map<string, double> val;
	for (auto it = sizes.begin(); it != sizes.end(); ++it)
	{
		val[it->first] = this->matchingTemplate(img, it->second);
	}

	// Select the min
	double min = pow(10, 7);
	for (auto it = val.begin(); it != val.end(); ++it)
	{
		if (it->second < min && it->second != 0)
		{
			min = it->second;
			value = it->first;
		}
	}

	return value;

}

double ImageExtraction::matchingTemplate(cv::Mat &img, cv::Mat &templ)
{

	Mat result;

	Mat img_display;
	img.copyTo(img_display);

	//threshold(img_display, img_display, 150, 255, THRESH_BINARY);

	/// Create the result matrix
	//int result_cols = img.cols - templ.cols + 1;
	//int result_rows = img.rows - templ.rows + 1;

	//result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	int match_method = CV_TM_SQDIFF;

	matchTemplate(img_display, templ, result, match_method);
	//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	//std::cout << minVal << " " << 2.8*pow(10, 7) << std::endl;

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	/// Show me what you got
	//rectangle(img, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	//rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	return minVal;// < 3 * pow(10, 6);
	//return 0;
}

