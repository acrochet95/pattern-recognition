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


vector<Mat> HSVDecomposition(const Mat& img)
{
	Mat img_hsv, img_threshold;
	cvtColor(img, img_hsv, CV_BGR2HSV);
	//namedWindow("win1", CV_WINDOW_AUTOSIZE);
	vector<Mat> hsv_channels;
	split(img_hsv, hsv_channels);

	return hsv_channels;

}

Mat myThresh(Mat img, double value){
	Mat res = img.clone();
	threshold(img, res, value, 255, THRESH_BINARY_INV);
	return res;
}

Mat extract(const Mat& img, Rect rc)
{
	Mat res(img, rc);
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
	//imshow("image reduite", imreduite);

	//computeHistogram("histogramme", im);

	vector<Mat> dec = HSVDecomposition(imreduite);
	Mat thresh = myThresh(dec[2],200);

	imshow("S Component thresh", thresh);

	vector<vector<cv::Point>> contours;

	findContours(thresh, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

	/*
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
