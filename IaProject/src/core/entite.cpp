#include "entite.h"


ENTITE::ENTITE(unsigned long id)
{
    this->id = id;
    mutationRate = 0;
    horlogeInterne = 0;
    dead = false;
    nbNeuronneCachePrevu = 0;
    nbSynapsesPrevu = 0;
    typeEntite = "undef_entity";

}

ENTITE::~ENTITE()
{
    for (int i = 0; i < NeuronnesEntree.size(); i++) {
        delete NeuronnesEntree[i];
    }
    for (int i = 0; i < Neuronnes.size(); i++) {
        delete Neuronnes[i];
    } 
    for (int i = 0; i < NeuronnesSortie.size(); i++) {
        delete NeuronnesSortie[i];
    }
}

void ENTITE::boucle()
{  
}

void ENTITE::chargerARN(string arnStr)
{
    arnEntite.importerARN(arnStr);
    chargerARN(arnEntite);
    genererARN();
}

void ENTITE::chargerARN(ARN arnToLoad)
{
    ///GENERATION DES NEURONNES
    int nbNeuronne = arnToLoad.getNbNeuronneCache();
    for (unsigned long i = 0; i < nbNeuronne; ++i) {
        neurons* newNeuronne = new neurons(arnToLoad.getHiddenNeuronnes()[i].id);
        newNeuronne->setActivationFunctionType(activationFunctionTypes(arnToLoad.getHiddenNeuronnes()[i].activation_type));
        Neuronnes.push_back(newNeuronne);
    }

    int nbSynapses = arnToLoad.getNbSynapse();
    for (unsigned long i = 0; i < nbSynapses; i++) {
        if (arnToLoad.getSynapses()[i].weight != 0) {
            findIDPtr(arnToLoad.getSynapses()[i].outputId)->addInput(arnToLoad.getSynapses()[i].inputId, arnToLoad.getSynapses()[i].weight, findIDPtr(arnToLoad.getSynapses()[i].inputId));
        }
    }
}

void ENTITE::genererARN()
{
    arnEntite.clear();
    for (int i = 0; i < Neuronnes.size(); i++) {
        vector<Input>& inputsN = Neuronnes[i]->getInputs();
        for (int k = 0; k < inputsN.size(); k++) {
            arnEntite.addSynapse(inputsN[k].getInputId(), Neuronnes[i]->getId(), inputsN[k].getWeight());
        }
    }
    for (int i = 0; i < NeuronnesSortie.size(); i++) {
        vector<Input>& inputsN = NeuronnesSortie[i]->getInputs();
        for (int k = 0; k < inputsN.size(); k++) {
            arnEntite.addSynapse(inputsN[k].getInputId(), NeuronnesSortie[i]->getId(), inputsN[k].getWeight());
        }
    }
    for (int i = 0; i < Neuronnes.size(); i++) {

        arnEntite.addNeuronneCachee(Neuronnes[i]->getId(), Neuronnes[i]->getActivationFunctionType());
    }
}

void ENTITE::muter()
{
    genererARN();
    vector<unsigned long> input;
    vector<unsigned long> output;
    for (int j = 0; j < NeuronnesEntree.size(); j++) {
        input.push_back(NeuronnesEntree[j]->getId());
    }
    for (int j = 0; j < NeuronnesSortie.size(); j++) {
        output.push_back(NeuronnesSortie[j]->getId());
        input.push_back(NeuronnesSortie[j]->getId());

    }
    for (int j = 0; j < Neuronnes.size(); j++) {
        input.push_back(Neuronnes[j]->getId());
        output.push_back(Neuronnes[j]->getId());
    }

    arnEntite.mutation(input, output,mutationRate, nbSynapsesPrevu, nbNeuronneCachePrevu);

}

neuronsEntre* ENTITE::findIDPtrE(unsigned long id)
{
    for (unsigned long i = 0; i < NeuronnesEntree.size(); i++) {
        if (NeuronnesEntree[i]->getId() == id) {
            return (NeuronnesEntree[i]);
        }
    }
}

neurons* ENTITE::findIDPtr(unsigned long id)
{
    for (unsigned long i = 0; i < Neuronnes.size(); i++) {
        if (Neuronnes[i]->getId() == id) {
            return Neuronnes[i];
        }
    }

    for (unsigned long i = 0; i < NeuronnesEntree.size(); i++) {
        if (NeuronnesEntree[i]->getId() == id) {
            return (NeuronnesEntree[i]);
        }
    }

    for (unsigned long i = 0; i < NeuronnesSortie.size(); i++) {
        if (NeuronnesSortie[i]->getId() == id) {
            return NeuronnesSortie[i];
        }
    }

    cout << "!NEURONNE INCONNU!" << endl;
    return nullptr;
}

void ENTITE::resetNeuronnes()
{
    for (int i = 0; i < NeuronnesSortie.size(); i++){
        NeuronnesSortie[i]->resetNeuronnes();        
    }
}

apparenceEntite ENTITE::getApparence()
{
    apparenceEntite apparence;
    apparence.apparenceChar = '$';
    apparence.imagePath = "";
    return apparence;
}

unsigned int ENTITE::getPosX()
{
    return 0;
}

unsigned int ENTITE::getPosY()
{
    return 0;
}

map<string, string> ENTITE::getData()
{
    map<string, string> res;
    res["entityType"] = typeEntite;
    res["id"] = to_string(id);
    res["mutationRate"] = to_string(mutationRate);
    res["internalClock"] = to_string(horlogeInterne);
    res["deadsate"] = to_string(dead);
    res["hiddenNeurons_Expected"] = to_string(nbNeuronneCachePrevu);
    res["synapses_Expected"] = to_string(nbSynapsesPrevu);
    for (int i = 0; i < NeuronnesEntree.size(); i++) {
        res["inN_" + NeuronnesEntree[i]->name + "(" + to_string(NeuronnesEntree[i]->getId()) + ")"] = to_string(NeuronnesEntree[i]->getValue());
    }
    for (int i = 0; i < NeuronnesSortie.size(); i++) {
        res["outN_" + NeuronnesSortie[i]->name + "(" + to_string(NeuronnesSortie[i]->getId()) + ")"] = to_string(NeuronnesSortie[i]->getValue());
    }
    return res;
}

void ENTITE::addNeuronneEntree(unsigned int id, string name, activationFunctionTypes typeActivation)
{
    neuronsEntre* nouveauNeuronne = new neuronsEntre(id);
    nouveauNeuronne->name = name;
    nouveauNeuronne->setActivationFunctionType(typeActivation);
    NeuronnesEntree.push_back(nouveauNeuronne);
}

void ENTITE::addNeuronneSortie(unsigned int id, string name, activationFunctionTypes typeActivation)
{
    neuronsSortie* nouveauNeuronne = new neuronsSortie(id);
    nouveauNeuronne->name = name;
    nouveauNeuronne->setActivationFunctionType(typeActivation);
    NeuronnesSortie.push_back(nouveauNeuronne);
}


