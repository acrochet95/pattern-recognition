#pragma once
class Icone
{
private:
	float iconWidth;
	float iconHeight;
	float iconSize;

public:
	float getIconWidth();
	float getIconHeight();
	float getIconSize();
	Icone();
	Icone(float iconH, float iconW, float size);
	~Icone();
};

