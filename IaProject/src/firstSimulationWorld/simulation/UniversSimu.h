#pragma once

#include "../map/mapIA.h"
#include "../basicEntity/EntiteDeBase.h"
#include <iostream>
#include <vector>

class UniversSimu
{
public:
	UniversSimu(unsigned int dimX, unsigned int dimY, float proportionFood);
	~UniversSimu();
	mapIA carte;

	void remplirMap(bool force = false);
	void setProportionNourriture(float);
	void supprEntite(unsigned long id);
	void supprEntite(unsigned long id, unsigned int x, unsigned int y);
	entity* findEntiteFromId(unsigned int id);
	std::vector<entity*> Ias;

protected:
	float proportionNourriture;


};

