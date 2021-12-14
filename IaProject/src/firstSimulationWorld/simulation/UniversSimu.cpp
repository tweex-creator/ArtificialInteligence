#include "UniversSimu.h"


UniversSimu::UniversSimu(unsigned int dimX, unsigned int dimY, float proportionFood):
	carte(dimX, dimY)
{
	cout << "what";
	proportionNourriture = proportionFood;
}

UniversSimu::~UniversSimu()
{
	for (int i = 0; i < Ias.size(); i++) {
		delete Ias[i];
	}
}
void UniversSimu::remplirMap(bool force)
{
	int nourritureDispo = 0;
	for (int c = 0; c < carte.getDimX(); c++) {
		for (int l = 0; l < carte.getDimY(); l++) {
			if (carte.caseData[c][l].nourritureDispo) {
				nourritureDispo++;
			}
		}
	}
	if (nourritureDispo > carte.getDimX() * carte.getDimY() * proportionNourriture+5) {
		cout << "suppose: " << carte.getDimX() * carte.getDimY() * proportionNourriture << "dispo: " << nourritureDispo << endl;
		for (int c = 0; c < carte.getDimX(); c++) {
			for (int l = 0; l < carte.getDimY(); l++) {
				if (carte.caseData[c][l].nourritureDispo) {
					carte.caseData[c][l].nourritureDispo = false;
				}
			}
		}
		nourritureDispo = 0;
	}
	for (int i = nourritureDispo; i < carte.getDimX() * carte.getDimY()* proportionNourriture and (i < nourritureDispo + 30 or force); i++) {
		int randPosX = rand() % carte.getDimX();
		int randPosY = rand() % carte.getDimY();
		while (carte.caseData[randPosX][randPosY].entitePtr.size() > 0) {
			randPosX = rand() % carte.getDimX();
			randPosY = rand() % carte.getDimY();
		}
		carte.caseData[randPosX][randPosY].nourritureDispo = true;
	}
}

void UniversSimu::setProportionNourriture(float nouvelleProportion)
{
	proportionNourriture = nouvelleProportion;
}

void UniversSimu::supprEntite(unsigned long id)
{
	carte.supprEntie(id);
	for (int i = 0; i < Ias.size(); i++) {
		if (id == Ias[i]->id) {
			auto elem_to_remove = Ias.begin() + i;
			delete Ias[i];
			Ias.erase(elem_to_remove);
			break;
		}
	}
}

void UniversSimu::supprEntite(unsigned long id, unsigned int x, unsigned int y)
{
	carte.supprEntie(id,x,y);

	for (int i = 0; i < Ias.size(); i++) {
		if (id == Ias[i]->id) {
			auto elem_to_remove = Ias.begin() + i;
			if (elem_to_remove != Ias.end()) {
				delete Ias[i];
				Ias.erase(elem_to_remove);
			}
		}
	}
}

ENTITE* UniversSimu::findEntiteFromId(unsigned int idEntite)
{
	for (int i = 0; i < Ias.size(); i++){
		if (Ias[i]->id == idEntite) {
			return Ias[i];
		}
	}
	return nullptr;
}
