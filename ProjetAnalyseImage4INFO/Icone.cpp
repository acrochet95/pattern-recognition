#include "stdafx.h"
#include "Icone.h"

Icone::Icone()
{
}

Icone::Icone(float iconW, float iconH, float s)
{
	iconWidth = iconW;
	iconHeight = iconH;
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

float Icone::getIconWidth()
{
	return iconWidth;
}

float Icone::getIconHeight()
{
	return iconHeight;
}

float Icone::getIconSize()
{
	return iconSize;
}

