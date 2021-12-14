#pragma once
#include "../../core/entite.h"

class caseMapIA {
public:
	caseMapIA();
	vector<ENTITE*> entitePtr;
	bool nourritureDispo;
	void supprEntie(unsigned long id);
};


class mapIA
{
public:
	mapIA(unsigned int sizeX, unsigned int sizeY);
	~mapIA();

	vector<vector<caseMapIA>> caseData;
	unsigned int getDimX();
	unsigned int getDimY();
	void supprEntie(unsigned long id);
	void supprEntie(unsigned long id, unsigned int x, unsigned int y);
private:
	unsigned int dimX;
	unsigned int dimY;
};

