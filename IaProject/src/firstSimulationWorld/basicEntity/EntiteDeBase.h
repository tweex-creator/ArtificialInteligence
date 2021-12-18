#pragma once
#include "../../core/entite.h"
#include "../map/mapIA.h"
#include <math.h>
#include <time.h>
#include<string>
#include <map>
using namespace std;
class EntiteDeBase :
    public entity
{
public:
	EntiteDeBase(unsigned int id, mapIA* map);
	~EntiteDeBase();
	void majEntree();
	void boucle();
	mapIA* map;
	int posX, posY;
	int lastPosX, lastPosY;
	unsigned int notMooving;
	int direction;// 0 = vers la droite, 1 bas, 2 gauche, 3 haut
	apparenceEntite getApparence();
	unsigned int getPosX();
	unsigned int getPosY();
	std::map<string, string> getData();

private:
	unsigned int apparence;
	int energie;
	int maxEnergie;



};

