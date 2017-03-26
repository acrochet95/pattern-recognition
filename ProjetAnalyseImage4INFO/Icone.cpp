#include "stdafx.h"
#include "Icone.h"

Icone::Icone(string type, string s)
{
	this->type = type;
	iconSize = s;
}

Icone::~Icone()
{
}

void Icone::setGravityCenters(vector<pair<int, int> > &gc)
{
	gravity_centers = gc;
}

void Icone::setBlackPixels(float bp)
{
	black_pixels = bp;
}

void Icone::setRatio(float r)
{
	ratio = r;
}

string Icone::getType()
{
	return type;
}

string Icone::getIconSize()
{
	return iconSize;
}

vector< pair<int, int > > Icone::getGravityCenters()
{
	return gravity_centers;
}

float Icone::getBlackPixels()
{
	return black_pixels;
}
float Icone::getRatio()
{
	return ratio;
}