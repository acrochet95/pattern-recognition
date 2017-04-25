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

void Icone::setRatiosBPx(vector<float> &rBPx)
{
	ratiosBPx = rBPx;
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

void Icone::setNbCorners(int nbC)
{
	nbCorners = nbC;
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

vector<float> Icone::getRatiosBPx()
{
	return ratiosBPx;
}

float Icone::getBlackPixels()
{
	return black_pixels;
}
float Icone::getRatio()
{
	return ratio;
}

int Icone::getNbCorners()
{
	return nbCorners;
}