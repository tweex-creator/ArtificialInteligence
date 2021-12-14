// projet IA avec evolution.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>

#include "src/firstSimulationWorld/simulation/simulation.h"
#include <fstream>
#include "src/firstSimulationWorld/sdl/sdlFirstIa.h"
using namespace std;


void printMap(simulation* simu) {

	std::cout << endl;

	for (int c = 0; c < simu->univers.carte.getDimX(); c++) {
		std::cout << "-";
	}
	std::cout << endl;

	for (int l = 0; l < simu->univers.carte.getDimY(); l++) {
		std::cout << "|";
		for (int c = 0; c < simu->univers.carte.getDimX(); c++) {
			if (simu->univers.carte.caseData[c][l].nourritureDispo) {
				std::cout << "*";
			}
			else if (simu->univers.carte.caseData[c][l].entitePtr.size() > 0) {
				std::cout << simu->univers.carte.caseData[c][l].entitePtr[0]->getApparence().apparenceChar;
			}
			else {
				std::cout << " ";
			}
		}
		std::cout << "|" << endl;
	}
	std::cout << " ";

	for (int c = 0; c < simu->univers.carte.getDimX(); c++) {
		std::cout << "-";
	}
	std::cout << endl << "generation: " << simu->gen << " | ";

	std::cout << "population: " << simu->univers.Ias.size() << " | ";
	std::cout << "synMoy: " << simu->nbMoySyn << " | ";
	std::cout << "NeMoy: " << simu->nbMoyNe << endl;
	std::cout << simu->refresh << endl;
}


int main(int argc, char* argv[])
{
	//sdlFirstIa sdlsimu(50, 50, 20, 0.0625, 0.15);
	sdlFirstIa sdlsimu(50, 50, 20, 0.0625, 0.1);
	//simulation simu(100,55);

	//bool aff = true;


	while (true) {
		sdlsimu.boucle();
		/*simu.boucle();

		if (aff) {
			system("CLS");
			printMap(&simu);
		}
		else if (simu.refresh % 1 == 0) {
			system("CLS");
			std::cout << endl << "generation: " << simu.gen << " | ";
			std::cout << "population: " << simu.univers.Ias.size() << " | ";
			std::cout << "synMoy: " << simu.nbMoySyn << " | ";
			std::cout << "NeMoy: " << simu.nbMoyNe << endl;
			std::cout << simu.refresh << endl;

		}
		*/

	}
	return 0;
}

