#include "ARN.h"
#include <iostream>
using namespace std;

vector<string> explode(const string& delimiter, const string& str)
{
	vector<string> arr;
	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng == 0)
		return arr;//no change

	int i = 0;
	int k = 0;
	while (i < strleng)
	{
		int j = 0;
		while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng)//found delimiter
		{
			arr.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back(str.substr(k, i - k));
	return arr;
}

ARN::ARN()
{
	nombreNeuronnesCaches = 0;
	nombreSynapses = 0;
	arnSize = 0;
}

void ARN::clear()
{
	synapses.clear();
	HiddenNeuronnes.clear();
}

bool ARN::addSynapse(unsigned long inputId, unsigned long outputId, float weight, bool force)
{
	bool SynapseAlreadyExist = false;
	int indice = 0;
	for (int i = 0; i < synapses.size(); i++) {
		if (inputId == synapses[i].inputId and outputId == synapses[i].outputId) {
			SynapseAlreadyExist = true;
			indice = i;
		}
	}
	
	if (SynapseAlreadyExist) {
		if (!force) {
			synapses[indice].weight = (synapses[indice].weight + weight) / float(2);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		arnSize++;
		ARNsynaps newSynapse;
		newSynapse.inputId = inputId;
		newSynapse.outputId = outputId;
		newSynapse.weight = weight;
		synapses.push_back(newSynapse);
		nombreSynapses = synapses.size();
	}
	return true;

}

void ARN::addNeuronneCachee(unsigned long int id, activationFunctionTypes activationType)
{
	ARNHiddenNeuronne newHiddenNeuron;
	newHiddenNeuron.id = id;
	newHiddenNeuron.activation_type = activationType;
	
	HiddenNeuronnes.push_back(newHiddenNeuron);
	nombreNeuronnesCaches = HiddenNeuronnes.size();
	arnSize++;
}

vector<ARNHiddenNeuronne>& ARN::getHiddenNeuronnes()
{
	return HiddenNeuronnes;
}

vector<ARNsynaps>& ARN::getSynapses()
{
	return synapses;
}

void ARN::importerARN(string ArnStr)
{
	vector<string> ARNarray = explode("|", ArnStr);
	///GENERATION DES NEURONNES
	unsigned long posArryay = 0;
	int nbNeuronne = stoi(ARNarray[posArryay]);
	posArryay++;

	for (unsigned long i = 0; i < nbNeuronne; ++i) {

		addNeuronneCachee(stoi(ARNarray[posArryay]), activationFunctionTypes(stoi(ARNarray[posArryay+1])));
		posArryay +=2;
		
	}

	int nbSynapses = stoi(ARNarray[posArryay]);
	posArryay++;

	for (unsigned long i = 0; i < nbSynapses; i++) {
		if (stoi(ARNarray[posArryay + 2]) != 0) {
			addSynapse(stoi(ARNarray[posArryay + 1]), stoi(ARNarray[posArryay]), stof(ARNarray[posArryay + 2]), false);		}
		posArryay += 3;
	}
}

string ARN::exportARN()
{
	arnSize = 0;

	string ARNstr = "";
	ARNstr = to_string(HiddenNeuronnes.size());
	for (int i = 0; i < HiddenNeuronnes.size(); i++) {
		arnSize++;
		ARNstr += "|" + to_string(HiddenNeuronnes[i].id) + "|" + to_string(HiddenNeuronnes[i].activation_type);
	}
	
	ARNstr += "|" + to_string(synapses.size());

	for (int i = 0; i < synapses.size(); i++) {
		arnSize++;
		ARNstr += "|" +  to_string(synapses[i].outputId) + "|" + to_string(synapses[i].inputId) + "|" + to_string(synapses[i].weight);
	}


	return ARNstr;
}

void ARN::mutation(vector<unsigned long> inputN, vector<unsigned long> outputN,float mutationRate, unsigned int maxSynaps, unsigned int maxNeuron)
{
	for (int i = 0; i < arnSize; i++) {

		if (mutationRate != 0 and rand() % int(float(1) / mutationRate) == 0) {
			int mutationType = rand() % 6;
			if (mutationType == 0) {// cree un nouveau synapse (Seulement si possible, voir addSynapse()
				if (getNbSynapse() < maxSynaps) {
					int idIn = rand() % inputN.size();
					int idOut = rand() % outputN.size();
					addSynapse(inputN[idIn], outputN.operator[](idOut), float(rand() % 800 - 400) / float(100));
				}
			}
			else if (mutationType == 1) {//modifie poids d'un synapse
				if (synapses.size() != 0) {
					int updateSynapsIndice = rand() % synapses.size();
					float oldWeight = synapses[updateSynapsIndice].weight;
					float newWeight = 0;
					do {
						newWeight = oldWeight + float(rand() % 100 - 50) / float(100);
					} while (newWeight < -4 or newWeight > 4);


					synapses[updateSynapsIndice].weight = newWeight;
				}
			}
			else if (mutationType == 2) {//modifie le neuronne d'entré ou de sortie d'un synapse
				if (synapses.size() != 0) {
					int updateSynapseIndice = rand() % synapses.size();
					int typeModif = rand() % 2;
					if (typeModif == 0) {
						int idIn = rand() % inputN.size();
						synapses[updateSynapseIndice].inputId = inputN[idIn];
					}
					else if (typeModif == 1) {
						int idOut = rand() % outputN.size();
						synapses[updateSynapseIndice].outputId = outputN.operator[](idOut);
					}
				}
			}
			else if (mutationType == 3) {//Modifie la fonction d'activation d'un neuronne
				if (HiddenNeuronnes.size() != 0) {
					int NeuronneModifieIndice = rand() % HiddenNeuronnes.size();
					HiddenNeuronnes[NeuronneModifieIndice].activation_type = activationFunctionTypes(rand() % 4);
				}
			}
			else if (mutationType == 4) {//supr hidden neuronne
				if (HiddenNeuronnes.size() != 0) {
					int neuronneSupIndice = rand() % HiddenNeuronnes.size();
					unsigned long idDuSupp = HiddenNeuronnes[neuronneSupIndice].id;
					auto it = synapses.begin();
					while (it != synapses.end()) {
						if (it->inputId == idDuSupp or it->outputId == idDuSupp) {
							it = synapses.erase(it);
						}
						else
						{
							it++;
						}
					}

					auto itIn = inputN.begin();
					while (itIn != inputN.end()) {
						if (*itIn == idDuSupp) {
							itIn = inputN.erase(itIn);
						}
						else
						{
							itIn++;
						}
					}

					auto itOut = outputN.begin();
					while (itOut != outputN.end()) {
						if (*itOut == idDuSupp) {
							itOut = outputN.erase(itOut);
						}
						else
						{
							itOut++;
						}
					}

					auto elem_to_remove = HiddenNeuronnes.begin() + neuronneSupIndice;

					if (elem_to_remove != HiddenNeuronnes.end()) {
						HiddenNeuronnes.erase(elem_to_remove);
					}
				}
			}
			else if (mutationType == 5) {//ajout d'un neuronne cachée
				if (getNbNeuronneCache() < maxNeuron) {
					unsigned int NewNeuronsId = 500;
					for (int i = 0; i < HiddenNeuronnes.size(); i++) {
						if (NewNeuronsId <= HiddenNeuronnes[i].id) {
							NewNeuronsId = HiddenNeuronnes[i].id + 1;
						}
					}
					addNeuronneCachee(NewNeuronsId, activationFunctionTypes(rand() % 4));

					if (getNbSynapse() < maxSynaps) {
						int idIn = rand() % inputN.size();
						addSynapse(inputN[idIn], NewNeuronsId, float(rand() % 800 - 400) / float(100));
					}
					if (getNbSynapse() < maxSynaps) {
						int idOut = rand() % outputN.size();
						addSynapse(NewNeuronsId, outputN.operator[](idOut), float(rand() % 800 - 400) / float(100));
					}

				}
			}
		}
	}
}

unsigned int ARN::getNbSynapse() const
{
	return synapses.size();
}

unsigned int ARN::getNbNeuronneCache() const
{
	return HiddenNeuronnes.size();
}
