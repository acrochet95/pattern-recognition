#include "stdafx.h"
#include "Icone.h"


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
