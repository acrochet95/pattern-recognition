//////////////////////////////////////////////////////////////////////////
// Module IRF, 5-iNFO
// Projet, séance 1
// thème : premier pas en OpenCV
// contenu : charge, affiche, réduction, calcul et affichage d'histogramme
// auteur : Luce Morin
// date : 4/10/2010
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "histogram.h"

#include "cv.h"
#include "highgui.h"

using namespace cv;

#include <iostream>
#include <random>
using namespace std;

#include "Icone.h"
#include "WriteARFF.h"

vector<Mat> split(Mat & img){
	vector<Mat> res;

	split(img, res);
	return res;
}


Mat toHSV(const Mat& img)
{
	Mat img_hsv;
	cvtColor(img, img_hsv, CV_BGR2HSV);
	return img_hsv;
}

Mat myThresh(Mat img, double value){
	Mat res = img.clone();
	threshold(img, res, value, 255, CV_THRESH_BINARY);
	return res;
}

Mat extract(const Mat& img, Rect rc)
{
	Mat res(img, rc);
	return res;
}

Mat gaussian(Mat &img, int size, double sigma){
	Mat res = img.clone();
	GaussianBlur(img, res, Size(size, size), sigma);
	return res;
}

Mat gray(Mat &img){
	Mat res;
	cv::cvtColor(img, res, CV_BGR2GRAY);
	return res;
}

Mat small(Mat &img, int reduction){

	Size tailleReduite(img.cols / reduction, img.rows / reduction);
	Mat imreduite = Mat(tailleReduite, CV_8UC3); //cree une image à 3 canaux de profondeur 8 bits chacuns
	resize(img, imreduite, tailleReduite);
	return imreduite;

}

int main(void) {
	/*
	//charge et affiche l'image 
	string imName = "..\\00000.png";
	Mat im = imread(imName);
	if (im.data == nullptr){
		cerr << "Image not found: " << imName << endl;
		waitKey(0);
		//system("pause");
		exit(EXIT_FAILURE);
	}

	/*
	Mat temp = gray(im);


	unsigned char ker[] = { 0, 0, 0, 1, 0, 0, 0,
							0, 1, 1, 1, 1, 1, 0,
							0, 1, 1, 1, 1, 1, 0,
							1, 1, 1, 1, 1, 1, 1,
							0, 1, 1, 1, 1, 1, 0,
							0, 1, 1, 1, 1, 1, 0,
							0, 0, 0, 1, 0, 0, 0, };
	Mat kernel = Mat(7, 7, CV_8U, (void*)ker);

	morphologyEx(temp, temp, MORPH_OPEN, kernel);
	temp = myThresh(temp, 250);

	imshow("binary", small(temp, 5));

	*/
	vector<vector<cv::Point>> contours;
	
	/*
	findContours(im, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	Mat drawing = Mat::zeros(im.size(), CV_8UC3);
	for (size_t i = 0; i< contours.size(); i++)
	{
	Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
	cv::drawContours(drawing, contours, (int)i, color);
	}
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	*/

	vector<Icone> icons;
	Icone icone1(2.0, 3.0, 5.0);
	Icone icone2(5.0, 7.0, 10.0);
	Icone icone3(12.0, 2.5, 5.6);
	icons.push_back(icone1);
	icons.push_back(icone2);
	icons.push_back(icone3);
	writeARFFFile(icons);

	//termine le programme lorsqu'une touche est frappee
	waitKey(0);
	//system("pause");
	return EXIT_SUCCESS;
}
