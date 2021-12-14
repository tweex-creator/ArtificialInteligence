#pragma once
#include "UniversSimu.h"
#include <iostream>
#include <fstream>
#include <time.h>
class simulation
{
public:
    simulation(unsigned int x, unsigned int y, float ratioNBIA, float proportionNourriture);

    string ARNgenerator(int nbHiden, int nbSynapses, vector<unsigned long>inputN, vector<unsigned long>outputN);

    bool loadFromSaveToBestARN(string emplacement);
    void save(string emplacement);

    void repopulerFromBestARN();
    void repopulerRandomARN();

    void avancerUneEtape();

    void getBestARNWithMutation();
    void clearPopulation();

    void boucle();

    unsigned int timeLeft();
    int NBIA;
   

    unsigned long refresh;
    unsigned long gen;
    unsigned long nbMoySyn;
    unsigned long nbMoyNe;
    string emplacementSauvegarde;
    UniversSimu univers;
    unsigned int nb_cycle_per_gen;
    unsigned int nb_cycle_running_gen;
    vector<string>bestArn;


};

