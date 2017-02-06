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



		file << "@ATTRIBUTE iconWidth NUMERIC" << endl;

		file << "@ATTRIBUTE iconHeight NUMERIC" << endl;

		file << "@ATTRIBUTE iconSize {small,medium,large}\n" << endl;

		file << "@DATA" << endl;

		for each (Icone icon in icons)
		{
			file << icon.iconWidth << "," << icon.iconHeight << "," << icon.iconSize  << endl;
		}

		file.close();

	}

	else

		cerr << "Impossible d'ouvrir le fichier !" << endl;
}