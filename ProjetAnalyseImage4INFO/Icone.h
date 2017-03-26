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
		void setBlackPixels(float bp);
		void setRatio(float r);

		string getType();
		string getIconSize();
		vector<pair<int, int> > getGravityCenters();
		float getBlackPixels();
		float getRatio();

	private:
		string type;
		string iconSize;
		vector<pair<int, int> > gravity_centers;
		float black_pixels;
		float ratio;
};

