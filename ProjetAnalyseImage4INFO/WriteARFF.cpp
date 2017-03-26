#include "stdafx.h"

#include "WriteARFF.h"
#include "Icone.h"

void writeARFFFile(vector<Icone> icons)
{

	string fileName = "Features.arff";
	ofstream file(fileName, ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert


	if (file)
	{

		file << "@RELATION IconsFeatures \n" << endl;

		file << "@ATTRIBUTE gravityCenterx NUMERIC" << endl;
		file << "@ATTRIBUTE gravityCentery NUMERIC" << endl;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{

				file << "@ATTRIBUTE gravityCenterZoning1x" << i << j << " NUMERIC" << endl;
				file << "@ATTRIBUTE gravityCenterZoning1y" << i << j << " NUMERIC" << endl;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{

				file << "@ATTRIBUTE gravityCenterZoning2x" << i << j << " NUMERIC" << endl;
				file << "@ATTRIBUTE gravityCenterZoning2y" << i << j << " NUMERIC" << endl;
			}
		}

		file << "@ATTRIBUTE blackPixels NUMERIC" << endl;

		file << "@ATTRIBUTE ratio NUMERIC" << endl;

		file << "@ATTRIBUTE iconSize {small,medium,large}\n" << endl;
		file << "@ATTRIBUTE type {accident,bomb,car,casualty,electricity,fire,fire_brigade,flood,gas,injury,paramedics,person,police,road_block}" << endl;

		file << "@DATA" << endl;

		for each (Icone icon in icons)
		{
			
			for (int i = 0; i < icon.getGravityCenters().size(); i++)
			{
				pair<int, int> pair = icon.getGravityCenters()[i];
				file << "," << pair.first << "," << pair.second;
			}

			file << "," << icon.getBlackPixels() << "," << icon.getRatio();

			file << "," << icon.getIconSize().c_str() << "," << icon.getType().c_str();

			file << endl;
		}

		file.close();

	}

	else

		cerr << "Impossible d'ouvrir le fichier !" << endl;
}