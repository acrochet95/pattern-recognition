#include "stdafx.h"
#include "FeaturesExtraction.h"

FeaturesExtraction::FeaturesExtraction()
{
	HANDLE hfind;
	WIN32_FIND_DATA wfd;

	// Liste le contenu du répertoire
	hfind = FindFirstFile("text\\*.*", &wfd);
	if (hfind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (wfd.cFileName[0] != '.')
			{
				cout << string(wfd.cFileName) << endl;
				files.push_back(string(wfd.cFileName));
			}

		} while (FindNextFile(hfind, &wfd));
		FindClose(hfind);
	}
}


FeaturesExtraction::~FeaturesExtraction()
{
}

void FeaturesExtraction::start()
{

	vector<Icone> icons;

	int a = 0, b=0;
	for (string filename : files)
	{
		a++;
		cout << a << " / " << files.size() << endl;
		// Explode
		std::vector<std::string> result;
		std::istringstream iss(filename);

		for (std::string token; std::getline(iss, token, '_');)
		{
			result.push_back(std::move(token));
		}

		// if it is good
		if (result.size() >= 5)
		{
			if (result.size() == 6)
			{
				std::vector<std::string> result2;
				result2.push_back(result[0] + "_" + result[1]);
				for (int i = 2; i < 6; i++)
					result2.push_back(result[i]);

				result = result2;
			}

			b++;
			string type = result[0];

			int id = stoi(result[3]) * 5 + stoi(result[4]) - 1;// +stoi(result[4]);
			string imName = "images/" + result[1] + "_" + result[2] + "_" + to_string(id) + "image.jpg";

			Mat img = imread(imName);
			if (img.data == nullptr){
				cerr << "Image not found: " << imName << endl;
				waitKey(0);

				exit(EXIT_FAILURE);
			}

			vector<pair<string, string>> description;
			string size;

			// LECTURE FICHIER TEXTE
			ifstream file("text\\"+filename);

			if (file)
			{
				//L'ouverture s'est bien passée, on peut donc lire
				string ligne; //Une variable pour stocker les lignes lues

				getline(file, ligne); // first line
				string first, second;
				while (file >> first >> second)
				{
					description.push_back(make_pair(first, second));
					//cout << first << " et " << second << endl;
				}

			}
			else
				cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;

			size = description[6].second;
			/*Mat dst, dst_norm, dst_norm_scaled, src_gray;
			dst = Mat::zeros(img.size(), CV_32FC1);

			cvtColor(img, src_gray, CV_BGR2GRAY);

			/// Detector parameters
			int blockSize = 2;
			int apertureSize = 3;
			double k = 0.04;

			/// Detecting corners
			cornerHarris(src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

			/// Normalizing
			normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
			convertScaleAbs(dst_norm, dst_norm_scaled);

			/// Drawing a circle around corners
			for (int j = 0; j < dst_norm.rows; j++)
			{
				for (int i = 0; i < dst_norm.cols; i++)
				{
					if ((int)dst_norm.at<float>(j, i) > 200)
					{
						circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
					}
				}
			}
			/// Showing the result

			char* corners_window = "Corners detected";
			//namedWindow(corners_window, CV_WINDOW_AUTOSIZE);
			imshow("okijgb", dst_norm_scaled);
			waitKey(0);
			//break;
			*/
			
			Icone icone(type, size);

			caract(img, icone);

			icons.push_back(icone);
			

		}
	}
	cout << b << endl;
	writeARFFFile(icons);
}

void FeaturesExtraction::caract(cv::Mat &img, Icone &icone)
{
	Mat binary, canny_img, corners, cornersNorm, cornersNormScaled;
	cvtColor(img, binary, CV_BGR2GRAY);

	corners = Mat::zeros(img.size(), CV_32FC1);

	// Counting edges with Harris corner detection :
	cornerHarris(binary, corners, 2, 3, 0.04, BORDER_DEFAULT);

	normalize(corners, cornersNorm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(cornersNorm, cornersNormScaled);

	int nbCorners = 0;

	for (int j = 0; j < cornersNorm.rows; j++)
	{
		for (int i = 0; i < cornersNorm.cols; i++)
		{
			if ((int)cornersNorm.at<float>(j, i) > 200)
			{
				nbCorners++;
			}
		}
	}
	cout << "Corner à la remoise : " << nbCorners << endl;
	icone.setNbCorners(nbCorners);

	threshold(binary, binary, 250, 255, THRESH_BINARY_INV);
	int TotalNumberOfPixels = binary.rows * binary.cols;
	int ZeroPixels = TotalNumberOfPixels - countNonZero(binary);

	// ratio pixels noir/ nb total pixel
	icone.setBlackPixels((float)ZeroPixels / (float)(img.cols * img.rows));



	//std::cout << ZeroPixels << " px black sur " << TotalNumberOfPixels << " px" << std::endl;
	vector<float> rBPx;
	vector<pair<int, int> > gc;

	vector<float> listBPx = zoningBlackPixels(2, binary);
	rBPx.insert(rBPx.end(), listBPx.begin(), listBPx.end());

	vector<float> listBPx2 = zoningBlackPixels(3, binary);
	rBPx.insert(rBPx.end(), listBPx2.begin(), listBPx2.end());

	icone.setRatiosBPx(rBPx);

	cv::Canny(binary, canny_img, 80, 240, 3);



	/*vector<Vec4i> lines;
	HoughLinesP(canny_img, lines, 1, CV_PI / 180, 50);
	//HoughLines(binary, lines, 1, CV_PI / 180, 150, 30);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	}
	*/

	//imshow("img" + rand() % 100, img);

	icone.setRatio(getRatio(canny_img));


	cv::Point gc_img = getCentroid(canny_img);

	gc.push_back(make_pair(gc_img.x, gc_img.y));

	vector<pair<int, int> > list = zoning(2, canny_img);
	gc.insert(gc.end(), list.begin(), list.end());

	vector<pair<int, int> > list2 = zoning(3, canny_img);
	gc.insert(gc.end(), list2.begin(), list2.end());

	icone.setGravityCenters(gc);
}


// return the ratio width/height of an icon
float FeaturesExtraction::getRatio(Mat& img)
{

	std::vector< std::vector< cv::Point> > contours;
	cv::findContours(img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	std::vector<std::vector<cv::Point> > contours_poly(contours.size());

	float x(100000), y(100000), x2(0), y2(0);
	for (int j = 0; j < contours.size(); j++)
	{

		cv::approxPolyDP(cv::Mat(contours[j]), contours_poly[j], 0, true);

		cv::Rect appRect(boundingRect(cv::Mat(contours_poly[j])));

		if (appRect.x < x)
			x = appRect.x;
		if (appRect.y < y)
			y = appRect.y;
		if (appRect.x + appRect.width > x2)
			x2 = appRect.x + appRect.width;
		if (appRect.y + appRect.height > y2)
			y2 = appRect.y + appRect.height;

	}

	return (x2 - x) / (y2 - y);
}

vector<float> FeaturesExtraction::zoningBlackPixels(int size, Mat &binary_img)
{
	const float boites_englobantes_largeur = size;
	const float boites_englobantes_hauteur = size;

	const float boite_largeur = (float)binary_img.size().width / boites_englobantes_largeur;
	const float boite_hauteur = (float)binary_img.size().height / boites_englobantes_hauteur;

	vector<float > ratios_BPx;

	for (int i = 0; i < boites_englobantes_largeur; i++)
	{
		for (int j = 0; j < boites_englobantes_hauteur; j++)
		{

			//cout << i << " / " << j << endl;
			cv::Rect rect(i*boite_largeur, j*boite_hauteur, boite_largeur, boite_hauteur);
			cv::Mat croppedFaceImage(binary_img, rect);

			int TotalNumberOfPixels = croppedFaceImage.rows * croppedFaceImage.cols;
			int ZeroPixels = TotalNumberOfPixels - countNonZero(croppedFaceImage);

			// ratio pixels noir/ nb total pixel
			float ratio = ((float)ZeroPixels / (float)(croppedFaceImage.cols * croppedFaceImage.rows));

			ratios_BPx.push_back(ratio);
		}
	}

	return ratios_BPx;

}

// Compute the gravity centers and the ratio : black pixels/total nbPixels   for each zone of the zoning
vector<pair<int, int> > FeaturesExtraction::zoning(int size, Mat &canny_img)
{
	const float boites_englobantes_largeur = size;
	const float boites_englobantes_hauteur = size;

	const float boite_largeur = (float)canny_img.size().width / boites_englobantes_largeur;
	const float boite_hauteur = (float)canny_img.size().height / boites_englobantes_hauteur;


	vector<pair<int, int> > gravity_centers;


	//cvtColor(img, binary, CV_BGR2GRAY);
	//cv::Canny(binary, canny_img, 80, 240, 3);
	//cv::Point gc_img = getCentroid(canny_img);
	//cout << gc_img.x << " et " << gc_img.y << endl;
	//circle(img, gc_img, 3, Scalar(255, 0, 0), CV_FILLED);
	//imshow("img" + rand() % 100, img);

	//gravity_centers.push_back(make_pair(gc_img.x, gc_img.y));

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
			//circle(croppedFaceImage, gc, 3, Scalar(255, 255, 255), CV_FILLED);
			//imshow("img" + rand() % 100, croppedFaceImage);

			gravity_centers.push_back(make_pair(gc.x, gc.y));
		}
	}

	return gravity_centers;
}

cv::Point FeaturesExtraction::getCentroid(cv::Mat img)
{
	cv::Point Coord;
	cv::Moments mm = cv::moments(img, false);
	double moment10 = mm.m10;
	double moment01 = mm.m01;
	double moment00 = mm.m00;
	Coord.x = int(moment10 / moment00);
	Coord.y = int(moment01 / moment00);

	//Enlever les valeurs erronées
	if (Coord.x <= -2147483644 || Coord.y <= -2147483644)
	{
		Coord.x = 0;
		Coord.y = 0;
	}
	return Coord;
}