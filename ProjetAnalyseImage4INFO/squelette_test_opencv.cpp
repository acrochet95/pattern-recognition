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

#include "ImageExtraction.h"
#include "FeaturesExtraction.h"

#include "WriteARFF.h"


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

	vector<cv::Mat> res;



	split(img, res);

	return res;

}





cv::Mat toHSV(const cv::Mat& img)

{

	cv::Mat img_hsv;

	cvtColor(img, img_hsv, CV_BGR2HSV);

	return img_hsv;

}



cv::Mat myThresh(cv::Mat img, double value){

	cv::Mat res = img.clone();

	threshold(img, res, value, 255, CV_THRESH_BINARY);

	return res;

}



cv::Mat extract(const cv::Mat& img, Rect rc)

{

	cv::Mat res(img, rc);

	return res;

}



cv::Mat gaussian(cv::Mat &img, int size, double sigma){

	cv::Mat res = img.clone();

	GaussianBlur(img, res, Size(size, size), sigma);

	return res;

}



cv::Mat gray(cv::Mat &img){

	cv::Mat res;

	cv::cvtColor(img, res, CV_BGR2GRAY);

	return res;

}


/*
cv::Mat small(cv::Mat &img, int reduction){



	Size tailleReduite(img.cols / reduction, img.rows / reduction);

	cv::Mat imreduite = cv::Mat(tailleReduite, CV_8UC3); //cree une image à 3 canaux de profondeur 8 bits chacuns

	resize(img, imreduite, tailleReduite);

	return imreduite;



}*/

void extractImages()
{
	vector<Icone> icons;


	//charge et affiche l'image 


	std::clock_t start;
	start = std::clock();

	ImageExtraction tr;
	int n = 0;
	//for (int j = 1; j < 34; j++)
	//Stoppé à dossier 29, Feuille 9
	for (int j = 2; j <= 34; j++)
	{
		cout << "######################## Dossier n°" << j << endl;
		stringstream folder;
		folder << setw(3) << setfill('0') << j;
		string f = folder.str();

		for (int i = 2; i <= 22; i++)
			//for (int i = 20; i < 21; i++)
		{
			cout << "# Scan n° " << i << endl;
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

			vector<Mat> images = tr.prepareImage(im); // Divise l'image en 7 pour obtenir les 7 lignes de dessins

			for (int i = 0; i < images.size(); i++)
			{

				vector<Mat> rects = tr.extractionImages(images[i]); // Extrait chaque dessin et le sauvegarde dans rects
				/*if (rects.size() < 5) { // Afficher les infos sur le dessin qui n'a pas été extrait 

				}*/
				n += rects.size();


				string icon = tr.matchingIcon(images[i]);

				string size = tr.matchingSize(images[i]);

				for (int a = 0; a < rects.size(); a++)
				{
					//Icone icone(0, 0, size);
					//cout << rects[a].size().width << endl;

					//imshow("img" + rand() % 100, rects[a]);
					cv::Mat croppedFaceImage;// (rects[i]);//images[i], rects[a]);
					rects[a].copyTo(croppedFaceImage);


					int id = i * 5 + a;
					imwrite("images/" + f + "_" + s + "_" + to_string(id) + "image.jpg", croppedFaceImage);
					writeFile(icon, f, s, i, a + 1, size);

					//tr.caract(croppedFaceImage, icone);


					//icons.push_back(icone);
				}
				//cout << icon << " " << size << endl;

				//icone.setBlackPixels()
			}
		}
	}

	//writeARFFFile(icons);

	int nb_img_theo = 34 * 20 * 35;


	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	cout << "time: " << duration << endl;

	cout << "total: " << n << " / " << nb_img_theo << endl;
	cout << "ratio: " << (double)n / (double)nb_img_theo << endl;

	//termine le programme lorsqu'une touche est frappee
	waitKey(0);
}

void extractFeatures()
{
	FeaturesExtraction fe;
	fe.start();
}


int main(void) {

	//extractImages();
	extractFeatures();

	return EXIT_SUCCESS;
}

