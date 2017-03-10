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

#include "TextRecognition.h"



#include "cv.h"

#include "highgui.h"

#include "highgui.hpp"
#include "imgproc.hpp"



using namespace cv;
using namespace cv::text;



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



	//charge et affiche l'image 


	std::clock_t start;
	start = std::clock();

	TextRecognition tr;
	int n = 0;
	//for (int j = 1; j < 34; j++)
	for (int j = 1; j < 2; j++)
	{
		stringstream folder;
		folder << setw(3) << setfill('0') << j;
		string f = folder.str();

		for (int i = 2; i < 3; i++)// 22; i++)
		{
			stringstream ss;
			ss << setw(2) << setfill('0') << i;
			string s = ss.str();

			string imName = "..\\ImageAnalyseImage\\w" + f + "-scans\\" + f + s + ".png";

			Mat im = imread(imName);
			if (im.data == nullptr){
				cerr << "Image not found: " << imName << endl;
				waitKey(0);

				exit(EXIT_FAILURE);
			}

			vector<Mat> images = tr.prepareImage(im);

			for (int i = 0; i < images.size(); i++)
			{

				vector<Rect> rects = tr.extractionImages(images[i]);
				n += rects.size();


				string icon = tr.matchingIcon(images[i]);

				string size = tr.matchingSize(images[i]);

				for (int a = 0; a < rects.size(); a++)
				{
					cv::Mat croppedFaceImage(images[i], rects[a]);
					images[i](rects[a]).copyTo(croppedFaceImage);

					int id = i * 5 + a;
					imwrite("images/" + f + "_" + s + "_" + to_string(id) + "image.jpg", croppedFaceImage);

					tr.caract(croppedFaceImage);

					//cout << "images/" + f + "_" + s + "_" + to_string(id) + "image.jpg" << endl;


				}
				cout << icon << " " << size << endl;
				writeFile(icon, f, s, 7 - i / 5, i % 5 + 1, size);
			}
		}
	}

	int nb_img_theo = 34 * 20 * 35;


	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	cout << "time: " << duration << endl;

	cout << "total: " << n << " / " << nb_img_theo << endl;
	cout << "ratio: " << (double)n / (double)nb_img_theo << endl;


	//termine le programme lorsqu'une touche est frappee
	waitKey(0);


	return EXIT_SUCCESS;

}

