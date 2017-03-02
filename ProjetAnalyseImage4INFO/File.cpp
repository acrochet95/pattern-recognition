#include "stdafx.h"
#include "File.h"

void writeFile(string iconID, string scripterNumber, string pageNumber, int row, int col, string size)
{
	string fileName = "text/" + iconID + "_" + scripterNumber + "_" + pageNumber + "_" + to_string(row) + "_" + to_string(col) + ".txt";
	ofstream file(fileName, ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert

	if (file)
	{
		file << "#PROJET 4INFO ANALYSE IMAGE - GROUPE : Geffrault Luc, Crochet Antoine, Harabari Victor" << endl;

		file << "label " << iconID << endl;
		file << "form " << scripterNumber + pageNumber << endl;
		file << "scripter " << scripterNumber << endl;
		file << "page " << pageNumber << endl;
		file << "row " << row << endl;
		file << "column " << col << endl;
		file << "size " << size << endl;

		file.close();
	}
	else
		cerr << "Impossible d'ouvrir le fichier !" << endl;

}

