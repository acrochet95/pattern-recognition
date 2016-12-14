#include "stdafx.h"
#include "File.h"


void writeFile(string iconID, int row, int col, string text, string size)
{
	string scripterNumber(text);
	scripterNumber.erase(text.length() - 2, text.length());

	string pageNumber(text);
	scripterNumber.erase(0, text.length()-2);

	string fileName = iconID + "_" + scripterNumber + "_" + pageNumber + "_" + to_string(row) + "_" + to_string(col) + ".txt";
	ofstream file(fileName, ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
	
	if (file)
	{
		file << "#PROJET 4INFO ANALYSE IMAGE - GROUPE : Geffrault Luc, Crochet Antoine, Harabari Victor" << endl;
		
		file << "label " << iconID << endl;
		file << "form " << text << endl;
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
