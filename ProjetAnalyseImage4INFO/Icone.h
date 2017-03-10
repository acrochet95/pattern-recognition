#pragma once
#include<iostream>
#include<vector>

using namespace std;

class Icone
{
	public:
		Icone();
		Icone(float iconH, float iconW, float size);
		~Icone();
		void setGravityCenters(vector<pair<int, int> > &gc);
		void setBlackPixels(float bp);
		float getIconWidth();
		float getIconHeight();
		float getIconSize();

	private:
		float iconWidth;
		float iconHeight;
		float iconSize;
		vector<pair<int, int> > gravity_centers;
		float black_pixels;
};

