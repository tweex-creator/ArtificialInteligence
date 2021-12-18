#include "Neuronne.h"
#include <math.h>
neurons::neurons(unsigned long id):nbActivationFunctions(4)
{
	this->id = id;
	activationFunctionType = activationFunctionTypes::tanhyperbolique;
	valeur = 0;
	reCalcul = true;
}

void neurons::addInput(unsigned int id, float weight, neurons* ptr)
{
	Input newInput;
	newInput.setInputId(id);
	newInput.setWeight(weight);
	newInput.setInputPtr(ptr);

	inputs.push_back(newInput);
}

float neurons::activationFunction(activationFunctionTypes activationFunctionParam, float val)
{
	float res = 0;
	if(activationFunctionParam == activationFunctionTypes::heaviside) {
		if (val >= 0) {
			res = 1;
		}
		else{
			res = 0;
		}
	}
	else if (activationFunctionParam == activationFunctionTypes::sigmoide) {
		res = float(1) / (1 + exp(-val));
	}
	else if (activationFunctionParam == activationFunctionTypes::tanhyperbolique) {
		res = tanh(val);
	}
	else if (activationFunctionParam == activationFunctionTypes::gauss) {
		res = exp(-(val * val));
	}
	else if (activationFunctionParam == activationFunctionTypes::rampe) {
		if (val > 1) {
			res = 1;
			cout << "WARNING: activationFonctionRampe > 1" << endl;
		}
		else if (val < -1) {
			res = -1;
			cout << "WARNING: activationFonctionRampe > 1" << endl;

		}
		else {
			res = val;

		}
	}
	return res;
}

float neurons::derivatedActivationFunction(activationFunctionTypes activationFunctionParam, float val)
{
	float res = 0;
	if (activationFunctionParam == activationFunctionTypes::heaviside) {
		res = 0;
	}
	else if (activationFunctionParam == activationFunctionTypes::sigmoide) {
		float sigmoVal = activationFunction(activationFunctionTypes::sigmoide, val);
		res = sigmoVal * (1 - sigmoVal);
	}
	else if (activationFunctionParam == activationFunctionTypes::tanhyperbolique) {
		float tanhVal = activationFunction(activationFunctionTypes::tanhyperbolique, val);

		res = 1-(tanhVal* tanhVal);
	}
	else if (activationFunctionParam == activationFunctionTypes::gauss) {
		float gaussVal = activationFunction(activationFunctionTypes::gauss, val);

		res = -2*val*gaussVal;
	}
	else if (activationFunctionParam == activationFunctionTypes::rampe) {	
		res= 1;		
	}
	return res;
}

float neurons::getValue()
{
	if (reCalcul) {
		reCalcul = false;
		float nextValue = 0;
		float sumInputsWeight = 0;
		for (int i = 0; i < inputs.size(); i++) {
			nextValue += inputs[i].getInputPtr()->getValue() * inputs[i].getWeight();
			sumInputsWeight += abs(inputs[i].getWeight());
		}
		valeur = activationFunction(activationFunctionType, nextValue/sumInputsWeight);

	}
	return valeur;
}

void neurons::resetNeurons()
{
	
	for (int i = 0; i < inputs.size(); i++) {
		if (reCalcul == false) {
			reCalcul = true;
			inputs[i].getInputPtr()->resetNeurons();
		}
	}

}

void neurons::setActivationFunctionType(activationFunctionTypes activationFunctionParam)
{
		activationFunctionType = activationFunctionParam;
}

activationFunctionTypes neurons::getActivationFunctionType()
{
	return activationFunctionType;
}

float neurons::backPropagation(float expected)
{
	float err = valeur - expected;
	return 0.0f;
}

unsigned long neurons::getId()
{
	return id;
}

vector<Input>& neurons::getInputs() 
{
	return inputs;
}

float neuronsEntre::getValue()
{
	return valeur;
}

neuronsEntre::neuronsEntre(unsigned int id):neurons(id)
{

}

void neuronsEntre::setValeur(float valeur) {
	this->valeur = valeur;
	//cout << endl<< "modif d'un input: " << this->valeur << endl;

}

void neuronsEntre::resetNeurons()
{
}

neuronsSortie::neuronsSortie(unsigned int id):neurons(id)
{
}

