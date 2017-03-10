#include "stdafx.h"
#include "TextRecognition.h"



TextRecognition::TextRecognition()
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

TextRecognition::~TextRecognition()
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
std::vector<cv::Rect> TextRecognition::detectLetters(cv::Mat img)

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

std::vector<cv::Mat> TextRecognition::prepareImage(Mat &img)
{
	std::vector<cv::Mat> values;
	cv::resize(img, img, cv::Size(707, 1000));

	//Mat img_gray;
	//cvtColor(img, img, CV_BGR2GRAY);

	//Mat img_display(img, Rect(0, 200, 707, 700));
	Mat img_display(img, Rect(0, 190, 707, 707));

	const int height = img_display.size().height / 7;
	for (int i = 0; i < 7; i++)
	{
		Mat im(img_display, Rect(0, i*(height), img_display.size().width, height));
		values.push_back(im);

		//imshow("im" + rand() % 100, im);
	}

	return values;
}

std::vector<cv::Rect> TextRecognition::extractionImages(cv::Mat &img)
{
	std::vector<cv::Rect> boundRect;

	cv::Mat img_gray, bw, img_sobel, img_threshold, element;


	//cv::resize(img, img, cv::Size(707, 1000));

	//cvtColor(img, img_gray, CV_BGR2GRAY);


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
			if (appRect.width > 70 && appRect.width < 80 && appRect.height > 70 && appRect.height < 80)
			{
				// Rognage pour enlever les bords
				const int rognage = 3;
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
				}

			}
		}
	}

	return boundRect;
}


std::string TextRecognition::matchingIcon(cv::Mat img)
{
	//std::vector<std::string> values;
	//cv::resize(img, img, cv::Size(707, 1000));
	//Mat img_display(img, Rect(0, 200, 160, 700));


	cvtColor(img, img, CV_BGR2GRAY);

	std::vector<std::string> elements;

	//const int height = 700 / 7;
	//for (int i = 0; i < 7; i++)
	//{
		string value = "";
		//Mat im(img_display, Rect(0, i*(height), img_display.size().width, height));

		//imshow("img" + rand() % 100, img);

		std::map<string, double> val;
		for (auto it = icons.begin(); it != icons.end(); ++it)
		{
			val[it->first] = this->matchingTemplate(img, it->second);
		}

		 // Select the min
		double min = pow(10,7);
		for (auto it = val.begin(); it != val.end(); ++it)
		{
			if (it->second < min && it->second != 0)
			{
				min = it->second;
				value = it->first;
			}
		}

		//values.push_back(value);
	//}
	
	return value;
}

std::string TextRecognition::matchingSize(cv::Mat img)
{
	//std::vector<std::string> values;
	//cv::resize(img, img, cv::Size(707, 1000));
	//Mat img_display(img, Rect(0, 200, 160, 700));


	cvtColor(img, img, CV_BGR2GRAY);

	std::vector<std::string> elements;

	//const int height = 700 / 7;
	//for (int i = 0; i < 7; i++)
	//{
	string value = "";
	//Mat im(img_display, Rect(0, i*(height), img_display.size().width, height));

	//imshow("img" + rand() % 100, img);

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

	//values.push_back(value);
	//}

	return value;


	/*
	std::vector<std::string> values;
	cv::resize(img, img, cv::Size(707, 1000));
	Mat img_display(img, Rect(0, 200, 160, 700));


	std::vector<std::string> elements;

	string value;
	const int height = 700 / 7;
	for (int i = 0; i < 7; i++)
	{
		string value = "";
		Mat im(img_display, Rect(0, i*(height), img_display.size().width, height));

		std::map<string, double> val;
		for (auto it = sizes.begin(); it != sizes.end(); ++it)
		{
			val[it->first] = this->matchingTemplate(im, it->second);
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

		values.push_back(value);
	}

	return values;*/
}

double TextRecognition::matchingTemplate(cv::Mat &img, cv::Mat &templ)
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

	//imshow("img2", img);
	//imshow("img", img_display);
	//imshow("result", result);
	return minVal;// < 3 * pow(10, 6);
	//return 0;
}

void TextRecognition::caract(cv::Mat &img)
{
	Icone icone;

	Mat binary, canny_img;
	cvtColor(img, binary, CV_BGR2GRAY);
	threshold(binary, binary, 240, 255, CV_THRESH_BINARY);
	int TotalNumberOfPixels = binary.rows * binary.cols;
	int ZeroPixels = TotalNumberOfPixels - countNonZero(binary);

	//imshow("img" + rand() % 100, binary);

	//std::cout << ZeroPixels << " px black sur " << TotalNumberOfPixels << " px" << std::endl;

	const float boites_englobantes_largeur = 2;
	const float boites_englobantes_hauteur = 2;

	const float boite_largeur = (float)img.size().width / boites_englobantes_largeur;
	const float boite_hauteur = (float)img.size().height / boites_englobantes_hauteur;
	


	vector<pair<int, int> > gravity_centers;


	//cvtColor(img, binary, CV_BGR2GRAY);
	cv::Canny(binary, canny_img, 80, 240, 3);
	cv::Point gc_img = getCentroid(canny_img);
	cout << gc_img.x << " et " << gc_img.y << endl;
	circle(img, gc_img, 3, Scalar(255, 0, 0), CV_FILLED);
	//imshow("img" + rand() % 100, img);

	gravity_centers.push_back(make_pair(gc_img.x, gc_img.y));

	for (int i = 0; i < boites_englobantes_largeur; i++)
	{
		for (int j = 0; j < boites_englobantes_hauteur; j++)
		{
			//cout << i << " / " << j << endl;
			cv::Rect rect(i*boite_largeur, j*boite_hauteur, boite_largeur, boite_hauteur);
			cv::Mat croppedFaceImage(canny_img, rect);
			//imshow("img" + rand() % 100, croppedFaceImage);
			//images[i](rects[a]).copyTo(croppedFaceImage);
			
			//cv::Canny(croppedFaceImage, croppedFaceImage, 80, 240, 3);
			cv::Point gc = getCentroid(croppedFaceImage);
			circle(croppedFaceImage, gc, 3, Scalar(255, 255, 255), CV_FILLED);
			imshow("img" + rand() % 100, croppedFaceImage);

			gravity_centers.push_back(make_pair(gc.x, gc.y));
		}
	}


}


cv::Point TextRecognition::getCentroid(cv::Mat img)
{
	cv::Point Coord;
	cv::Moments mm = cv::moments(img, false);
	double moment10 = mm.m10;
	double moment01 = mm.m01;
	double moment00 = mm.m00;
	Coord.x = int(moment10 / moment00);
	Coord.y = int(moment01 / moment00);
	return Coord;
}

