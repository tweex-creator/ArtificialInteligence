#include "mapIA.h"

caseMapIA::caseMapIA() {
	nourritureDispo = 0;
}

void caseMapIA::supprEntie(unsigned long id)
{
	for (int i = 0; i < entitePtr.size(); i++) {
		if (id == entitePtr[i]->id) {
			auto elem_to_remove = entitePtr.begin() + i;
			if (elem_to_remove != entitePtr.end()) {
				entitePtr.erase(elem_to_remove);
			}
		}
	}
	
}

mapIA::mapIA(unsigned int sizeX, unsigned int sizeY)
{
	cout << "Generation map" << endl;

	dimX = sizeX;
	dimY = sizeY;
	for (int c = 0; c < dimX; c++) {
		vector<caseMapIA> colone;
		for (int l =  0; l < dimY; l++) {
			colone.push_back(caseMapIA());
		}
		caseData.push_back(colone);
	}

	for (int c = 0; c < dimX; c++) {
		for (int l = 0; l < dimY; l++) {
			caseData[c][l].nourritureDispo = 0;
		}
	}
	cout << "Map Generee" << endl;
}

mapIA::~mapIA()
{
	
}

unsigned int mapIA::getDimX()
{
	return dimX;
}

unsigned int mapIA::getDimY()
{
	return dimY;
}

void mapIA::supprEntie(unsigned long id)
{
	for (int c = 0; c < getDimX(); c++) {
		for (int l = 0; l < getDimY(); l++) {
			caseData[c][l].supprEntie(id);
		}
	}
}

void mapIA::supprEntie(unsigned long id, unsigned int x, unsigned int y)
{
	caseData[x][y].supprEntie(id);

}
