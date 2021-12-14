#pragma once
#include <vector>
#include <string>
#include "Neuronne.h"
using namespace std;
struct ARNsynaps {
	unsigned long inputId;
	unsigned long outputId;
	float weight;
};
struct ARNHiddenNeuronne {
	unsigned long int id;
	activationFunctionTypes activation_type;
};


class ARN
{
public:
	ARN();
	void clear();
	bool addSynapse(unsigned long inputId, unsigned long outputId, float weight, bool force = false);
	void addNeuronneCachee(unsigned long int id, activationFunctionTypes activationType);
	vector<ARNHiddenNeuronne>& getHiddenNeuronnes();
	vector<ARNsynaps>& getSynapses();
	void importerARN(string ArnStr);
	string exportARN();
	void mutation(vector<unsigned long> inputN, vector<unsigned long> outputN, float mutationRate, unsigned int maxSynaps, unsigned int maxNeuron);
	unsigned int getNbSynapse() const;
	unsigned int getNbNeuronneCache() const;
	unsigned int arnSize;
private:
	int nombreSynapses;
	int nombreNeuronnesCaches;
	vector<ARNHiddenNeuronne> HiddenNeuronnes;
	vector<ARNsynaps> synapses;
};