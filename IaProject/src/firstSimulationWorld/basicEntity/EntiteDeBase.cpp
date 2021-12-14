#include "EntiteDeBase.h"
#include <math.h> 
EntiteDeBase::EntiteDeBase(unsigned int id, mapIA* map):ENTITE(id)
{
	this->map = map;
	notMooving = 0;
	apparence = 0;
	energie = 0;
	typeEntite = "Gogb (premiere Ia)";
	posX = rand() % map->getDimX();
	posY = rand() % map->getDimY();
	lastPosX = posX;
	lastPosY = posY;
	direction = rand() % 4;
	energie = 90;
	maxEnergie = 200;
	nbNeuronneCachePrevu = 6;
	nbSynapsesPrevu = 60;
	mutationRate = 0.025;

	addNeuronneEntree(0, "energieLVL", activationFunctionTypes::rampe);
	addNeuronneEntree(1, "foodDetector", activationFunctionTypes::rampe);
	addNeuronneEntree(2, "entityDetector", activationFunctionTypes::rampe);
	addNeuronneEntree(3, "wallDetector", activationFunctionTypes::rampe);
	addNeuronneEntree(4, "cycle5", activationFunctionTypes::rampe);
	addNeuronneEntree(5, "pulse5", activationFunctionTypes::rampe);
	addNeuronneEntree(6, "foodAtRight", activationFunctionTypes::rampe);
	addNeuronneEntree(7, "foodAtLeft", activationFunctionTypes::rampe);

	inputN.push_back(0);
	inputN.push_back(1);
	inputN.push_back(2);
	inputN.push_back(3);
	inputN.push_back(4);
	inputN.push_back(5);
	inputN.push_back(6);	
	inputN.push_back(7);
	addNeuronneSortie(100, "avancer", activationFunctionTypes::tanhyperbolique);
	addNeuronneSortie(101, "tournerD", activationFunctionTypes::tanhyperbolique);
	addNeuronneSortie(102, "tournerG", activationFunctionTypes::tanhyperbolique);
	addNeuronneSortie(103, "manger", activationFunctionTypes::tanhyperbolique);

	outputN.push_back(100);
	outputN.push_back(101);
	outputN.push_back(102);
	outputN.push_back(103);

}
EntiteDeBase::~EntiteDeBase() {
	ENTITE::~ENTITE();
}
void EntiteDeBase::boucle()
{

	if (!dead) {
		horlogeInterne++;
		energie -= 1;
		//cout << energie << endl;
		
		resetNeuronnes();
		majEntree();
		float resTournerD = findIDPtr(101)->getValue();
		if (resTournerD >= 0.5) {
			direction++;
			direction = direction % 4;
			if (direction < 0) {
				direction = 3;
			}

		}

		resetNeuronnes();
		majEntree();
		float resTournerG = findIDPtr(102)->getValue();
		if (resTournerG >= 0.5) {
			direction--;
			direction = direction % 4;
			if (direction < 0) {
				direction = 3;
			}
		}

		resetNeuronnes();
		majEntree();
		float resAvancer = findIDPtr(100)->getValue();
		if(resAvancer >= 0.5) {
			energie -= 3;
			if (direction == 0) {
				if (posX < map->getDimX() - 1 and map->caseData[posX + 1][posY].entitePtr.size() == 0) {
					posX++;
				}
			}
			if (direction == 1) {
				if (posY < map->getDimY() - 1 and map->caseData[posX][posY + 1].entitePtr.size() == 0) {
					posY++;
				}
			}
			if (direction == 2) {
				if (posX > 0 and map->caseData[posX - 1][posY].entitePtr.size() == 0) {
					posX--;
				}
			}
			if (direction == 3) {

				if (posY > 0 and map->caseData[posX][posY - 1].entitePtr.size() == 0) {
					posY--;
				}
			}
		}


		

		resetNeuronnes();
		majEntree();
		float resManger = findIDPtr(103)->getValue();
		if (resManger >= 0.5) {
			if (map->caseData[posX][posY].nourritureDispo and energie < maxEnergie) {
				energie += 100;
				map->caseData[posX][posY].nourritureDispo = false;
			}
			else {
				energie -= 10;

			}
			
		}

		if (map->caseData[posX][posY].nourritureDispo and energie < maxEnergie) {
			energie += 30;
			map->caseData[posX][posY].nourritureDispo = false;
		}

		if (energie <= 0.1) {
			dead = true;
			energie = 0.001;
		}

	}
	if (dead) {
		apparence = 0;
	}
	else if (energie < 50) {
		apparence = 1;
	}
	else if (energie <= 100) {
		apparence = 2;
	}
	else if (energie > 100) {
		apparence = 3;
	}
	else {
		apparence = 4;
	}

	if (posX != lastPosX or lastPosY != posY) {
		notMooving = 0;
		map->supprEntie(id);
		map->caseData[posX][posY].entitePtr.push_back(this);
	}
	else {
		notMooving++;
		bool isInCase = false;
		for (int i = 0; i < map->caseData[posX][posY].entitePtr.size(); i++) {
			if (map->caseData[posX][posY].entitePtr[i] == this) {
				isInCase = true;
			}
		}
		if (!isInCase) {
			map->caseData[posX][posY].entitePtr.push_back(this);
		}
	}
	if (notMooving > 5) {
		//energie -= 4;
	}
	lastPosX = posX;
	lastPosY = posY;

	resetNeuronnes();
	majEntree();
}

apparenceEntite EntiteDeBase::getApparence()
{
	apparenceEntite res;

	if (apparence == 0) {
		res.apparenceChar = 'D';
		res.imagePath = "data/assets/mort.jpg";

	}
	else if (apparence == 1) {
		res.apparenceChar = 'I';
		res.imagePath = "data/assets/pacManRouge.jpg";
	}
	else if (apparence == 2) {
		res.apparenceChar = 'X';
		res.imagePath = "data/assets/pacManBleu.jpg";
	}
	else if (apparence == 3) {
		res.apparenceChar = 'O';
		res.imagePath = "data/assets/pacManJaune.jpg";
	}
	else {
		res.apparenceChar = '?';
		res.imagePath = "data/assets/unknow.jpg";
	}
	return res;
}

void EntiteDeBase::majEntree()
{
	//Energie Level
	findIDPtrE(0)->setValeur(float(energie) / float(maxEnergie));

	//FOOD detector
	float distance = 99;
	for (int i = -5; i < 5; i++) {
		for (int j = -5; j < 5; j++) {
			if ((posX + i > 0 and posX + i < map->getDimX()) and posY + j > 0 and posY + j < map->getDimY()) {
				if (map->caseData[posX + i][posY + j].nourritureDispo) {
					if (sqrtf(i * i + j * j) < distance && sqrtf(i * i + j * j) != 0) {
						distance = sqrtf(i*i + j*j);
					}
				}
			}
		}
	}
	if (distance == 99) {
		findIDPtrE(1)->setValeur(0);
	}
	else {
		distance = 1-(float(distance)/sqrtf(50));
		findIDPtrE(1)->setValeur(distance);
	}

	//ENTITYDETECTOR
	if (direction == 0) {
		if (posX < map->getDimX() - 1 and map->caseData[posX+1][posY].entitePtr.size() == 0) {
			findIDPtrE(2)->setValeur(0);
		}
		else {
			findIDPtrE(2)->setValeur(1);
		}
	}
	else if (direction == 1) {
		if (posY < map->getDimY() - 1 and map->caseData[posX][posY+1].entitePtr.size() == 0) {
			findIDPtrE(2)->setValeur(0);
		}
		else {
			findIDPtrE(2)->setValeur(1);
		}
	}
	else if (direction == 2) {
		if (posX > 0 and map->caseData[posX-1][posY].entitePtr.size() == 0) {
			findIDPtrE(2)->setValeur(0);
		}
		else {
			findIDPtrE(2)->setValeur(1);
		}
	}
	else if (direction == 3) {
		if (posY > 0 and map->caseData[posX][posY - 1].entitePtr.size() == 0) {
			findIDPtrE(2)->setValeur(0);
		}
		else {
			findIDPtrE(2)->setValeur(1);
		}
	}
	else {
		findIDPtrE(2)->setValeur(0);
	}

	//wallDetector
	if (direction == 0) {
		if (posX == map->getDimX() - 1) {
			findIDPtrE(3)->setValeur(1);
		}
	}
	if (direction == 1) {
		if (posY == map->getDimY() - 1) {
			findIDPtrE(3)->setValeur(1);
		}
	}
	if (direction == 2) {
		if (posX == 0) {
			findIDPtrE(3)->setValeur(1);
		}
	}
	if (direction == 3) {

		if (posY == 0) {
			findIDPtrE(3)->setValeur(1);
		}
	}
	else {
		findIDPtrE(3)->setValeur(0);
	}

	//cycle5
	if (horlogeInterne % 10 < 5) {
		findIDPtrE(4)->setValeur(1);
	}
	else {
		findIDPtrE(4)->setValeur(0);
	}
	
	//pulse5
	if (horlogeInterne % 5 == 0) {
		findIDPtrE(5)->setValeur(1);
	}
	else {
		findIDPtrE(5)->setValeur(0);
	}

	//foodAtRight
	if (direction == 0) {
		if (posY < map->getDimY()-1 and map->caseData[posX][posY + 1].nourritureDispo) {
			findIDPtrE(6)->setValeur(1);
		}
		else {
			findIDPtrE(6)->setValeur(0);

		}
	}
	if (direction == 1) {
		if (posX > 0 and map->caseData[posX-1][posY].nourritureDispo) {
			findIDPtrE(6)->setValeur(1);
		}
		else {
			findIDPtrE(6)->setValeur(0);

		}
	}
	if (direction == 2) {
		if (posY > 0 and map->caseData[posX][posY - 1].nourritureDispo) {
			findIDPtrE(6)->setValeur(1);
		}
		else {
			findIDPtrE(6)->setValeur(0);

		}
	}
	if (direction == 3) {

		if (posX < map->getDimX() - 1 and map->caseData[posX+1][posY].nourritureDispo) {
			findIDPtrE(6)->setValeur(1);
		}
		else {
			findIDPtrE(6)->setValeur(0);

		}
	}
	
	//foodAtLeft

	if (direction == 0) {
		if (posY > 0 and map->caseData[posX][posY - 1].nourritureDispo) {
			findIDPtrE(7)->setValeur(1);
		}
		else {
			findIDPtrE(7)->setValeur(0);

		}
	}
	if (direction == 1) {

		if (posX < map->getDimX() - 1 and map->caseData[posX + 1][posY].nourritureDispo) {
			findIDPtrE(7)->setValeur(1);
		}
		else {
			findIDPtrE(7)->setValeur(0);

		}
	}
	if (direction == 2) {
		if (posY < map->getDimY() - 1 and map->caseData[posX][posY + 1].nourritureDispo) {
			findIDPtrE(7)->setValeur(1);
		}
		else {
			findIDPtrE(7)->setValeur(0);

		}
	}
	if (direction == 3) {
		if (posX > 0 and map->caseData[posX - 1][posY].nourritureDispo) {
			findIDPtrE(6)->setValeur(1);
		}
		else {
			findIDPtrE(6)->setValeur(0);

		}
	}
	
	
}

unsigned int EntiteDeBase::getPosX()
{
	return posX ;
}

unsigned int EntiteDeBase::getPosY()
{
	return posY;
}

map<string, string> EntiteDeBase::getData()
{
	std::map<string, string> res = ENTITE::getData();
	res["posX"] = to_string(posX);
	res["posY"] = to_string(posY);
	res["lastPosX"] = to_string(lastPosX);
	res["lastPosY"] = to_string(lastPosY);
	res["notMooving"] = to_string(notMooving);
	res["direction"] = to_string(direction);
	res["energie"] = to_string(energie);
	res["apparence"] = getApparence().apparenceChar;
	res["energie_Max"] = to_string(maxEnergie);
	return res;
}
