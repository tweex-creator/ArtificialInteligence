#include "simulation.h"
#include <algorithm>

simulation::simulation(unsigned int x, unsigned int y, float rationNBIA, float proportionNourriture):univers(x,y, proportionNourriture)
{
    srand(time(NULL));
    emplacementSauvegarde = "";
    this->NBIA = rationNBIA*float(x*y);
    refresh = 0;
    gen = 0;
    nbMoyNe = 0;
    nbMoySyn = 0;
	nbMoySyn = 40;


	nb_cycle_running_gen = 0;
	nb_cycle_per_gen = 100;
	if (loadFromSaveToBestARN("save/sauvegarde.txt")) {
		repopulerFromBestARN();
	}
	else {
		repopulerRandomARN();
	}

}

string simulation::ARNgenerator(int nbHiden, int nbSynapses, vector<unsigned long>inputN, vector<unsigned long>outputN) {
	ARN arn;

	for (unsigned long i = 0; i < nbHiden; i++) {
		arn.addNeuronneCachee(500 + i, activationFunctionTypes(rand() % 4));
		inputN.push_back(500 + i);
		outputN.push_back(500 + i);
	}

	for (int i = 0; i < nbSynapses; i++) {
		int idIn;
		int idOut;

		do {
			idIn = rand() % inputN.size();
			idOut = rand() % outputN.size();
		} while (!arn.addSynapse(inputN[idIn], outputN[idOut], rand() % 8 - 4, true));
	}
	return arn.exportARN();
}

bool simulation::loadFromSaveToBestARN(string emplacement)
{
	ifstream monFlux(emplacement.c_str());
	bestArn.clear();
	if (monFlux)
	{

		string ligne; //Une variable pour stocker les lignes lues
		if (getline(monFlux, ligne)) {
			gen = stoi(ligne);
		}
		while (getline(monFlux, ligne)) //Tant qu'on n'est pas à la fin, on lit
		{
			bestArn.push_back(ligne);
		}
	}
	else
	{
		std::cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
	}
	if (bestArn.size() == 0) {
		return false;
	}
	else {
		return true;
	}
}

void simulation::save(string emplacement)
{
	ofstream monFlux(emplacement.c_str(), ios::out);

	if (monFlux)
	{
		monFlux << gen << endl;
		for (int i = 0; i < univers.Ias.size(); i++) {
			monFlux << univers.Ias[i]->arnEntite.exportARN() << endl;
		}
	}
	else
	{
		std::cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
	}
}


void simulation::repopulerFromBestARN()
{
	for (int i = 0; i < NBIA; i++) {
		EntiteDeBase* nouvelleEntite = new EntiteDeBase(i, &univers.carte);
		univers.Ias.push_back(nouvelleEntite);
		univers.Ias[i]->loadARN(bestArn[i % bestArn.size()]);
		nbMoySyn += univers.Ias[i]->arnEntite.getNbSynapse();
		nbMoyNe += univers.Ias[i]->arnEntite.getNbNeuronneCache();
	}
	nbMoySyn = nbMoySyn / univers.Ias.size();
	nbMoyNe = nbMoyNe / univers.Ias.size();
	

}

void simulation::repopulerRandomARN()
{
	for (int i = 0; i < NBIA; i++) {
		EntiteDeBase* nouvelleEntite = new EntiteDeBase(i, &univers.carte);
		string arn = ARNgenerator(nouvelleEntite->nbNeuronneCachePrevu, nouvelleEntite->nbSynapsesPrevu, nouvelleEntite->inputN, nouvelleEntite->outputN);
		univers.Ias.push_back(nouvelleEntite);
		univers.Ias[i]->loadARN(arn);
	}

}

void simulation::avancerUneEtape()
{
	unsigned int countIasBoucle = 0;
	while (countIasBoucle < univers.Ias.size()) {
		univers.Ias[countIasBoucle]->boucle();
		if (univers.Ias[countIasBoucle]->dead){
			univers.supprEntite(univers.Ias[countIasBoucle]->id);

		}
		else {
			countIasBoucle++;
		}
	}
}

void simulation::getBestARNWithMutation()
{

	bestArn.clear();
	if (univers.Ias.size() != 0) {
		for (int i = 0; i < NBIA; i++) {
			univers.Ias[i % univers.Ias.size()]->muter();
			bestArn.push_back(univers.Ias[i % univers.Ias.size()]->arnEntite.exportARN());
		}
	}
	else {
		for (int i = 0; i < NBIA; i++) {
			EntiteDeBase nouvelleEntite(i, &univers.carte);
			string arn = ARNgenerator(nouvelleEntite.nbNeuronneCachePrevu, nouvelleEntite.nbSynapsesPrevu, nouvelleEntite.inputN, nouvelleEntite.outputN);
			bestArn.push_back(arn);
		}
	}
}

void simulation::clearPopulation()
{
	while (univers.Ias.size() != 0) {
		univers.supprEntite(univers.Ias[0]->id);
	}
}

void simulation::boucle()
{
	avancerUneEtape();
	if (nb_cycle_running_gen == 0) {
		univers.remplirMap(true);
	}
	else {
		univers.remplirMap(false);
	}
	refresh++;
	nb_cycle_running_gen++;
	//if (nb_cycle_running_gen > nb_cycle_per_gen) {
	if (univers.Ias.size() < NBIA/10) {

		nb_cycle_running_gen=0;
		std::cout << "MUTATION...";
		save("./save/sauvegarde.txt");
		getBestARNWithMutation();
		clearPopulation();
		repopulerFromBestARN();
		gen++;

	}
}
