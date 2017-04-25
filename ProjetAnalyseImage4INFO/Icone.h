#pragma once
#include<iostream>
#include<vector>

using namespace std;

class Icone
{
	public:
		Icone(string type, string size);
		~Icone();
		void setGravityCenters(vector<pair<int, int> > &gc);
		void setRatiosBPx(vector<float> &rBPx);
		void setBlackPixels(float bp);
		void setRatio(float r);
		void setNbCorners(int nbC);

		string getType();
		string getIconSize();
		vector<pair<int, int> > getGravityCenters();
		vector<float> getRatiosBPx();
		float getBlackPixels();
		float getRatio();
		int getNbCorners();

	private:
		string type;
		string iconSize;
		vector<pair<int, int> > gravity_centers;
		vector<float> ratiosBPx;
		float black_pixels;
		float ratio;
		int nbCorners;
};

