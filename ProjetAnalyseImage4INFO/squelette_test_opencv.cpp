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


int main (void) {

	//charge et affiche l'image 
	string imName = "..\\00000.png";
	Mat im = imread(imName);
	if(im.data == nullptr){
		cerr << "Image not found: "<< imName << endl;
		waitKey(0);
		//system("pause");
		exit(EXIT_FAILURE);
	}
	//imshow("exemple1", im);


	//applique une reduction de taille d'un facteur 5
	//ici modifier pour ne reduire qu'a l'affichage 
	//comme demande dans l'enonce
	int reduction = 5;
	Size tailleReduite(im.cols/reduction, im.rows/reduction);
	Mat imreduite = Mat(tailleReduite,CV_8UC3); //cree une image à 3 canaux de profondeur 8 bits chacuns
	resize(im,imreduite,tailleReduite);
	imshow("image reduite", imreduite);

	//computeHistogram("histogramme", im);

	Mat temp = imreduite.clone();

	temp = toHSV(temp);

	vector<Mat> dec = split(temp);
	
	namedWindow("H", WINDOW_AUTOSIZE);
	imshow("H", dec[0]);
	namedWindow("S", WINDOW_AUTOSIZE);
	imshow("S", dec[1]);
	namedWindow("V", WINDOW_AUTOSIZE);
	imshow("V", dec[2]);
	
	/*
	vector<vector<cv::Point>> contours;

	findContours(thresh, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	Mat drawing = Mat::zeros(imreduite.size(), CV_8UC3);
	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
		cv::drawContours(drawing, contours, (int)i, color);
	}
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);*/

	//termine le programme lorsqu'une touche est frappee
	waitKey(0);
	//system("pause");
	return EXIT_SUCCESS;
}
