#pragma once
#include "ARN.h"
#include "Neuronne.h"
#include<string>
#include <map>
using namespace std;

struct apparenceEntite {
	char apparenceChar;
	string imagePath;
};


class entity
{
public:
	/**
		Intialise une entite avec un Id (qui ne peut pas être modifié par la suite)
	*/
	entity(unsigned long id);
	~entity();
	
	/**
	Crée les neuronnes et synapses de l'entite en fonction de l'ARN passée en parametre
	*/
	void loadARN(string ARN);
	void loadARN(ARN ARN);

	/**
		Met a jour l'ARN de l'entite (see: arnEntite)
	*/
	void generateARN();
	
	/**
	Fait muter l'Arn de l'entite
	*/
	void muter();
	
	/**
	Les vecterus contenant les pointeurs vers les de neuronne de l'entite
	*/
	vector<neuronsEntre*> NeuronnesEntree;
	vector<neurons*> Neuronnes;
	vector<neuronsSortie*> NeuronnesSortie;

	/**
	Fonction  qui retourne l'ensemble des données sur l'entite
	*/
	virtual map<string, string> getData();

	/**
		Fonction  qui retourne un pointeur sur le neuronne d'entre dont l'id est passé en paramètre, nullPtr si il n'existe pas
	*/
	neuronsEntre* findIDPtrE(unsigned long id);
	/**
		Fonction  qui retourne un pointeur sur le neuronne dont l'id est passé en paramètre, nullPtr si il n'existe pas
	*/
	neurons* findIDPtr(unsigned long id);
	
	/**
		Remet a zero l'ensembles des neuronnes de sortie de l'entite et donc par propagation l'ensemble des neuronnes utilisés par l'entité
	*/
	void resetNeurons();

	/**
		A definir pour chaque type d'entite(classe herité de neurons). cette fonction definit l'ensemblre
	*/
	virtual void boucle();

	/**
		Renvoie l'apparence de l'entite pour l'affichage graphique ou consol
	*/
	virtual apparenceEntite getApparence();

	virtual unsigned int getPosX();
	virtual unsigned int getPosY();


	float mutationRate;
	bool dead;
	string typeEntite;
	vector<unsigned long>inputN;
	vector<unsigned long>outputN;
	unsigned int nbNeuronneCachePrevu;
	unsigned int nbSynapsesPrevu;

	unsigned long id;

	ARN arnEntite;


protected:
	unsigned long int horlogeInterne;
	void addNeuronneEntree(unsigned int id, string name, activationFunctionTypes typeActivation);
	void addNeuronneSortie(unsigned int id, string name, activationFunctionTypes typeActivation);
};

