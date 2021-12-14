#ifndef NEURONNE_C
#define NEURONNE_C

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
class neurons;

class Input
{
public:
	void setInputId(unsigned long inputNeuronneid);
	unsigned long getInputId();
	void setWeight(float weight);
	float getWeight();
	void setInputPtr(neurons* ptr);
	neurons* getInputPtr();
private:
	unsigned long inputNeuronneid;
	float weight;
	neurons* ptr;
};


inline void Input::setInputId(unsigned long inputNeuronneid)
{
	this->inputNeuronneid = inputNeuronneid;
}

inline unsigned long Input::getInputId()
{
	return inputNeuronneid;
}

inline void Input::setWeight(float weight)
{
	this->weight = weight;
}

inline float Input::getWeight()
{
	return weight;
}

inline void Input::setInputPtr(neurons* ptr)
{
	this->ptr = ptr;
}

inline neurons* Input::getInputPtr()
{
	return ptr;
}

enum activationFunctionTypes
{
	heaviside = 0,
	sigmoide = 1,
	tanhyperbolique = 2,
	gauss = 3,
	rampe = 4,
	relu = 5
};

class neurons
{
public:
	/**
		Intialise le Neuronne avec un Id (qui ne peut pas être modifié par la suite)
	*/
	neurons(unsigned long id);

	/**
		Ajoute un neuronne d'entree aux neuronne courant
	*/
	void addInput(unsigned int id, float weight, neurons* ptr);

	/**
	Permet de modifier le type d'activation du Neuronne
	*/
	void setActivationFunctionType(activationFunctionTypes activationFunctionParam);

	/**
		Afin de limiter le nombres de calcules par cycles, la valeur d'un Neuronne n'est recalculé que si il a etait remis a zero depuis le derniers cycle, sinon il renvoie directement la valeurs
		de la derniere valeur calculé.
		See: getValue();
	*/
	virtual void resetNeuronnes();

	/**
		retourne la valeur du neuronne calculée à partir des neuronnes d'entrées et de la fonction d'activation
	*/
	virtual float getValue();

	/**
		Permet de recuperer l'identifiant du Neuronne
	*/
	unsigned long getId();

	

	/**
		Permet de recuperer le type d'activation du Neuronne
	*/
	activationFunctionTypes getActivationFunctionType();

	
	/**
		Permet de recuperer une reference sur le vector qui contient l'ensemble des Input (see:Input)
	*/
	vector<Input>& getInputs() ;

	/**
		Applique la fonction d'activation du type passé en parametre sur la valeur val
	*/
	float activationFunction(activationFunctionTypes activationFunctionParam, float val);

protected:

	/**
	Contient l'enssemble des neuronnes d'entées associès au neuronne*/
	vector<Input> inputs;
	
	/**
	La dernière valeur calculé par le neuronne*/
	float valeur;

	/**Identifiant du neuronne*/
	unsigned long id;

	/**Le type d'activation du neuronne (voir activationFunction(...,...) */
	activationFunctionTypes activationFunction;

	/**Le nome de fonction d'activation disponible */
	const unsigned short nbActivationFunctions;

	/**Permet de savoir si la fonction doit être recalculé */
	bool reCalcul;

};

class neuronsEntre: public neurons
{
public:
	neuronsEntre(unsigned int id);
	string name;
	void setValeur(float valeur);
	float getValue();

protected:
	float valeur = 0;
	void resetNeuronnes();
};


class neuronsSortie : public neurons
{
public:
	neuronsSortie(unsigned int id);
	string name;

protected:
	float valeur = 0;
};


#endif