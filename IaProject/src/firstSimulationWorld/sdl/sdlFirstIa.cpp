#include "sdlFirstIa.h"
#include <algorithm>
#include <Windows.h>
using namespace std;
#define WIDTH 1024
#define HEIGHT 576
sdlFirstIa::sdlFirstIa(unsigned int x, unsigned int y,unsigned int zoom, float ratioNBIA, float proportionNourriture):simu(x,y, ratioNBIA, proportionNourriture)
{
    stepToDo = -1;
    pause = false;
    pauseAff = false;
    this->zoom = zoom;
    dimWinX = x * zoom;
    dimWinY = y * zoom;
    //dimWinX = 1000;
    //dimWinY = 1000;
    srand(time(NULL));
    init_SDL();
    tempo = 0;
    windowEntiteData = nullptr;
    affBrain = false;
    affInfo = false;
    idSelectedNeuron = 9999;


}

void sdlFirstIa::setFontSize(int size) {
    if (font != nullptr) {
        TTF_CloseFont(font);
    }

    font = TTF_OpenFont("data/fonts/Arial.ttf", size);

}

sdlFirstIa::~sdlFirstIa()
{
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void sdlFirstIa::init_SDL()
{
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }
    if (TTF_Init() != 0) {
        cout << "Erreur lors de l'initialisation de la SDL_ttf : " << TTF_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    setFontSize(24);
   

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    //creation de la fenetre : 
    window = SDL_CreateWindow("Simulation Ia ERUAM Herve", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dimWinX, dimWinY, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    //creation du rendu : 
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        cout << "Erreur lors de la creation du renderer : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }
}


void sdlFirstIa::boucle() {
    if (stepToDo == -1 or stepToDo > 0) {
    if (!pause) {
            simu.boucle();
            if (stepToDo > 0) {
                stepToDo--;
            }
        }
    }

    cout << "gen: " << simu.gen << " | pauseAff: " << pauseAff << endl;
    if (!pauseAff) {
        sdlAff();
    }
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_MOUSEBUTTONDOWN: 
            if (event.button.windowID = SDL_GetWindowID(window)) {
                for (int i = 0; i < entityHitbox.size(); i++) {
                    if (entityHitbox[i].is_in(event.button.x, event.button.y)) {
                        trackedEntityId = entityHitbox[i].entite->id;
                    }
                }
            }
            if (event.button.windowID = SDL_GetWindowID(windowEntiteBrain) && idSelectedNeuron == 9999) {
                auto it = neuronsAffHitbox.begin();
                while(it != neuronsAffHitbox.end()) {
                    if (it->second.is_in(event.button.x, event.button.y)) {
                        idSelectedNeuron = it->second.neuron->getId();
                        int hauteur = neuronsAffHitbox[idSelectedNeuron].getHauteur();
                        int x = 0;
                        int y = 0;
                        SDL_GetMouseState(&x, &y);
                        neuronsAffHitbox[idSelectedNeuron].x1 = x;
                        neuronsAffHitbox[idSelectedNeuron].y1 = y;
                        neuronsAffHitbox[idSelectedNeuron].x2 = x + hauteur * 2;
                        neuronsAffHitbox[idSelectedNeuron].y2 = y - hauteur * 2;
                        it = neuronsAffHitbox.end();
                    }
                    else {
                        it++;
                    }
                }
            }
            break;
        case SDL_MOUSEBUTTONUP: 
            if (event.button.windowID = SDL_GetWindowID(windowEntiteBrain) && idSelectedNeuron != 9999) {
                int hauteur = neuronsAffHitbox[idSelectedNeuron].getHauteur();
                int x = 0;
                int y = 0;
                SDL_GetMouseState(&x, &y);
                neuronsAffHitbox[idSelectedNeuron].x1 = x;
                neuronsAffHitbox[idSelectedNeuron].y1 = y;
                neuronsAffHitbox[idSelectedNeuron].x2 = x + hauteur;
                neuronsAffHitbox[idSelectedNeuron].y2 = y - hauteur;
                idSelectedNeuron = 9999;
            }
            break;
        case SDL_MOUSEMOTION:
            if (event.motion.windowID = SDL_GetWindowID(windowEntiteBrain) && idSelectedNeuron != 9999) {
                int hauteur = neuronsAffHitbox[idSelectedNeuron].getHauteur();
                int x = 0;
                int y = 0;
                SDL_GetMouseState(&x, &y);
                neuronsAffHitbox[idSelectedNeuron].x1 = x;
                neuronsAffHitbox[idSelectedNeuron].y1 = y;
                neuronsAffHitbox[idSelectedNeuron].x2 = x + hauteur;
                neuronsAffHitbox[idSelectedNeuron].y2 = y - hauteur;
            }
            break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                pause = !pause;
                if (!pause) {
                    stepToDo = -1;
                }
                break;
            case SDLK_s: 
                pause = false;
                stepToDo = 1;
                break;
            case SDLK_b:
                affBrain = !affBrain;
                break;
            case SDLK_i:
                affInfo = !affInfo;
                break;
            case SDLK_p:
                pauseAff = !pauseAff;

                break;
            case SDLK_LEFT:
                if (tempo >= 10) {
                    tempo -= 10;

                }
                break; 
            case SDLK_RIGHT:

                tempo += 10;
                break;

            default: 
                break;
            }
            
        }
        break;

        case SDL_QUIT: {
            printf("Quit requested, quitting.\n");
            exit(0);
        }
        break;
        }
    }
    SDL_RenderPresent(renderer);
    Sleep(tempo);
}

void sdlFirstIa::entityBrainLoop()
{
}

void sdlFirstIa::initSimu()
{
}

void sdlFirstIa::affEntiteData()
{
    if (windowEntiteData == nullptr) {
        //creation de la fenetre : 
        windowEntiteData = SDL_CreateWindow("Donnees sur l'entite", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 800, SDL_WINDOW_SHOWN);
        if (windowEntiteData == nullptr)
        {
            cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
            SDL_Quit();
            exit(1);
        }

        //creation du rendu : 
        rendererEntiteData = SDL_CreateRenderer(windowEntiteData, -1, SDL_RENDERER_ACCELERATED);
        if (rendererEntiteData == nullptr)
        {
            cout << "Erreur lors de la creation du renderer : " << SDL_GetError() << endl;
            SDL_Quit();
            exit(1);
        }
    }
    
    entity* entity = simu.univers.findEntiteFromId(trackedEntityId);

    SDL_SetRenderDrawColor(rendererEntiteData, 255, 255, 255, 255);
    SDL_RenderClear(rendererEntiteData);
    SDL_Color color;
    color.r = 255;
    color.g = 0;
    string txt;


    unsigned int posYAff = 0;
    setFontSize(24);
    if (entity == nullptr) {
        txt = "Aucune entite selectionnée";
        sdlAffTexte(txt, posYAff, 0, 0, 0, color, rendererEntiteData);
        posYAff += 20;
        trackedEntityId = simu.univers.Ias[0]->id;

    }
    else {
        map<string, string> neuronData = entity->getData();
        for (auto it = neuronData.begin(); it != neuronData.end(); ++it)
        {
            txt = it->first + ": " + it->second;
            sdlAffTexte(txt, 0, posYAff, 0, 0, color, rendererEntiteData);
            posYAff += 20;
        }
    }
    txt = "population: " + to_string(simu.univers.Ias.size());
    sdlAffTexte(txt, 0, posYAff, 0, 0, color, rendererEntiteData);
    posYAff += 20;

    txt = "population initial: " + to_string(simu.NBIA);
    sdlAffTexte(txt, 0, posYAff, 0, 0, color, rendererEntiteData);
    posYAff += 20;

    txt = "gen: " + to_string(simu.gen);
    sdlAffTexte(txt, 0, posYAff, 0, 0, color, rendererEntiteData);
    posYAff += 20;

    txt = "avancement de la gen en cours: " + to_string(simu.nb_cycle_running_gen * 100 / simu.nb_cycle_per_gen) + "%";
    sdlAffTexte(txt, 0, posYAff, 0, 0, color, rendererEntiteData);
    posYAff += 20;
    SDL_RenderPresent(rendererEntiteData);

}

void sdlFirstIa::affEntiteBrain()
{

    const int winY = 800;
    const int winX = 1800;

    if (windowEntiteBrain == nullptr) {
        //creation de la fenetre : 
        windowEntiteBrain = SDL_CreateWindow("Cerveaux de l'entite", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winX, winY, SDL_WINDOW_SHOWN);
        if (windowEntiteBrain == nullptr)
        {
            cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
            SDL_Quit();
            exit(1);
        }

        //creation du rendu : 
        rendererEntiteBrain = SDL_CreateRenderer(windowEntiteBrain, -1, SDL_RENDERER_ACCELERATED);
        if (rendererEntiteBrain == nullptr)
        {
            cout << "Erreur lors de la creation du renderer : " << SDL_GetError() << endl;
            SDL_Quit();
            exit(1);
        }
    }

    entity* entity = simu.univers.findEntiteFromId(trackedEntityId);
    SDL_SetRenderDrawColor(rendererEntiteBrain, 255, 255, 255, 255);
    SDL_RenderClear(rendererEntiteBrain);
    SDL_Color color;
    color.r = 255;
    color.g = 0;
    string txt;

    if (entity != nullptr) {
        neuronsAffHitbox.clear();
        int NeuronSize = (winY - (entity->NeuronnesEntree.size() + 1) * 10) / entity->NeuronnesEntree.size();
        unsigned int posY = 10;
        unsigned int posX = 0;
        //Neuronnes entrées
        for (int i = 0; i < entity->NeuronnesEntree.size(); i++) {
            hitBoxNeurons neuronHitBox(10, 10 + NeuronSize, posY, posY + NeuronSize);
            neuronHitBox.neuron = entity->NeuronnesEntree[i];
            neuronsAffHitbox[entity->NeuronnesEntree[i]->getId()] = neuronHitBox;
            posY += NeuronSize + 10;
        }

        //Neuronnes cachés
        vector<neurons*> inputsNeurons;
        vector<neurons*> list;
        vector<neurons*> direct;
        vector<neurons*> reste;

        for (int i = 0; i < entity->Neuronnes.size(); i++) {
            reste.push_back(entity->Neuronnes[i]);
        }
        for (int i = 0; i < entity->NeuronnesEntree.size(); i++) {
            direct.push_back(entity->NeuronnesEntree[i]);
        }

        posY = 10;
        posX = 300;
        bool decalY = false;
        while (direct.size() != 0) {

            list.clear();
            inputsNeurons.clear();
            for (int i = 0; i < reste.size(); i++) {
                list.push_back(reste[i]);
            }
            for (int i = 0; i < direct.size(); i++) {
                inputsNeurons.push_back(direct[i]);
            }
            direct.clear();
            reste.clear();
            getNeuronDependance(&inputsNeurons, &list, &direct, &reste);

            if (direct.size() != 0) {
                NeuronSize = min(int((winY - (direct.size() + 1) * 10) / direct.size()), winY / 10);
                for (int i = 0; i < direct.size(); i++) {

                    hitBoxNeurons neuronHitBox(posX, posX + NeuronSize, posY, posY + NeuronSize);
                    neuronHitBox.neuron = direct[i];
                    neuronsAffHitbox[direct[i]->getId()] = neuronHitBox;
                    posY += NeuronSize + 10;
                }

            }
            else if(reste.size() != 0){
                NeuronSize = min(int((winY - (reste.size() + 1) * 10) / reste.size()), winY / 10);
                for (int i = 0; i < reste.size(); i++) {

                    hitBoxNeurons neuronHitBox(posX, posX + NeuronSize, posY, posY + NeuronSize);
                    neuronHitBox.neuron = reste[i];
                    neuronsAffHitbox[reste[i]->getId()] = neuronHitBox;
                    posY += NeuronSize + 10;
                }
            }


            if (!decalY) {
                posY = 10;
            }
            else {
                posY = 10 + NeuronSize / 2;
            }
            posX += 300;

            decalY = !decalY;

        }
        NeuronSize = min(int(((winY - (entity->NeuronnesSortie.size() + 1) * 10) / entity->NeuronnesSortie.size())), winY / 10);





        for (int i = 0; i < entity->NeuronnesSortie.size(); i++) {


            hitBoxNeurons neuronHitBox(winX - 10 - NeuronSize, winX - 10, posY, posY + NeuronSize);
            neuronHitBox.neuron = entity->NeuronnesSortie[i];
            neuronsAffHitbox[entity->NeuronnesSortie[i]->getId()] = neuronHitBox;
            posY += NeuronSize + 10;

        }



      

        for (auto it = neuronsAffHitbox.begin(); it != neuronsAffHitbox.end(); it++) {

            setFontSize(13);
            string path;
            if (it->second.neuron->getValue() > 0.5) {
                path = "data/assets/neuronAct.jpg";
            }
            else {
                path = "data/assets/neuronDesact.jpg";
            }
            sdlAffImgInHitBox(&getImage(path, rendererEntiteBrain), &it->second);
            txt = to_string(it->second.neuron->getId());
            sdlAffTexte(txt, it->second.x1 + it->second.getHauteur() / 7, it->second.y1 + it->second.getHauteur() / 6, 0, 0, color, rendererEntiteBrain);
            txt = to_string(it->second.neuron->getValue());
            sdlAffTexte(txt, it->second.x1 + it->second.getHauteur() / 7, it->second.y1 + 2 * it->second.getHauteur() / 6, 0, 0, color, rendererEntiteBrain);
            txt = "act: " + to_string(it->second.neuron->getActivationFunctionType());
            sdlAffTexte(txt, it->second.x1 + it->second.getHauteur() / 7, it->second.y1 + 4 * it->second.getHauteur() / 6, 0, 0, color, rendererEntiteBrain);



            int startX = it->second.x1;
            int startY = it->second.y1 + it->second.getHauteur()/2;

            for (int i = 0; i < it->second.neuron->getInputs().size(); i++) {
                if (neuronsAffHitbox.find(it->second.neuron->getInputs()[i].getInputId()) != neuronsAffHitbox.end()) {
                    int endX = neuronsAffHitbox[it->second.neuron->getInputs()[i].getInputId()].x1 + neuronsAffHitbox[it->second.neuron->getInputs()[i].getInputId()].getLargeur();
                    int endY = neuronsAffHitbox[it->second.neuron->getInputs()[i].getInputId()].y1 + neuronsAffHitbox[it->second.neuron->getInputs()[i].getInputId()].getHauteur() / 2;
                    if (it->second.neuron->getInputs()[i].getWeight() < 0) {
                        SDL_SetRenderDrawColor(rendererEntiteBrain, 255, 0, 0, SDL_ALPHA_OPAQUE);
                    }
                    else if (it->second.neuron->getInputs()[i].getWeight() > 0) {
                        SDL_SetRenderDrawColor(rendererEntiteBrain, 0, 255, 0, SDL_ALPHA_OPAQUE);
                    }
                    else {
                        SDL_SetRenderDrawColor(rendererEntiteBrain, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    }
                    SDL_RenderDrawLine(rendererEntiteBrain, startX, startY, endX, endY);
                    int w = abs((it->second.neuron->getInputs()[i].getWeight() / 4) * 10);
                    int j = 0;
                    int e = 1;
                    while (j < w) {
                        if (j % 2 == 0) {
                            SDL_RenderDrawLine(rendererEntiteBrain, startX, startY + e, endX, endY + e);
                        }
                        else {
                            SDL_RenderDrawLine(rendererEntiteBrain, startX, startY - e, endX, endY - e);
                            e++;
                        }
                        j++;
                    }
                }
            }

        }
    }
    else {
        txt = "Aucune entite selectionnée";
        sdlAffTexte(txt, 0, 0, 0, 0, color, rendererEntiteBrain);
        trackedEntityId = simu.univers.Ias[0]->id;
    }

    SDL_RenderPresent(rendererEntiteBrain);

}

void sdlFirstIa::getNeuronDependance(vector<neurons*>* inputsNeurons, vector<neurons*>* list, vector<neurons*>* direct, vector<neurons*>* reste)
{

    for (int i = 0; i < list->size(); i++) {

        bool isIn = false;
        for (int k = 0; k < list->operator[](i)->getInputs().size(); k++) {
            for (int j = 0; j < inputsNeurons->size(); j++) {
                if (!isIn and list->operator[](i)->getInputs()[k].getInputId() == inputsNeurons->operator[](j)->getId()) {
                    direct->push_back(list->operator[](i));
                    isIn = true;
                    break;
                }
            }
        }
        if (!isIn) {
            reste->push_back(list->operator[](i));
        }

    }



    
}


Image& sdlFirstIa::getImage(string path, SDL_Renderer* renderUse)
{
    auto search = im_entite.find(path);
    if (search == im_entite.end()) {
        im_entite[path].loadFromFile(path.c_str(), renderUse);
    }
    else if (search->second.renderer != renderUse) {
        im_entite[path].loadFromFile(path.c_str(), renderUse);
    }
    return im_entite[path];
}

void sdlFirstIa::sdlAff()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    sdlAffTerrain();
    if (affInfo) {
        affEntiteData();
    }
    else {
        if (rendererEntiteData != nullptr) {
            SDL_DestroyRenderer(rendererEntiteData);
            rendererEntiteData = nullptr;
        }
        if (windowEntiteData != nullptr) {
            SDL_DestroyWindow(windowEntiteData);
            windowEntiteData = nullptr;
        }
    }
    if (affBrain) {
        affEntiteBrain();
    }
    else {
        if (rendererEntiteBrain != nullptr) {
            SDL_DestroyRenderer(rendererEntiteBrain);
            rendererEntiteBrain = nullptr;
        }
        if (windowEntiteBrain != nullptr) {
            SDL_DestroyWindow(windowEntiteBrain);
            windowEntiteBrain = nullptr;
        }
    }
    

    SDL_RenderPresent(renderer);

}

void sdlFirstIa::sdlAffTerrain()
{

    entityHitbox.clear();
    for (int l = 0; l < simu.univers.carte.getDimY(); l++) {
      
        for (int c = 0; c < simu.univers.carte.getDimX(); c++) {
            if (simu.univers.carte.caseData[c][l].nourritureDispo) {
                getImage("data/assets/food.jpg", renderer).draw(renderer, c * zoom, l * zoom, zoom, zoom);
            }
            else if (simu.univers.carte.caseData[c][l].entitePtr.size() > 0) {
                hitBoxEntite entite(c * zoom, c * zoom + zoom, l * zoom, l * zoom + zoom);
                entite.entite = simu.univers.carte.caseData[c][l].entitePtr[0];
                entityHitbox.push_back(entite);
                if (simu.univers.carte.caseData[c][l].entitePtr[0]->id == trackedEntityId) {
                    getImage(simu.univers.carte.caseData[c][l].entitePtr[0]->getApparence().imagePath, renderer).draw(renderer, c * zoom - zoom/2, l * zoom - zoom/ 2, zoom*2, zoom*2);

                }
                else {
                    getImage(simu.univers.carte.caseData[c][l].entitePtr[0]->getApparence().imagePath, renderer).draw(renderer, c * zoom, l * zoom, zoom, zoom);
                }
            }
        }
    }
   


}

void sdlFirstIa::sdlAffTexte(const string& txt, const int& x, const int& y, const int& largeur, const int& hauteur, const SDL_Color& couleur, SDL_Renderer* renderer) {
    SDL_Surface* texte = nullptr;
    SDL_Rect position;
    SDL_Texture* texte_texture = nullptr;    //Create Texture pointeur

    texte = TTF_RenderText_Blended(font, txt.c_str(), font_color);
    
    texte_texture = SDL_CreateTextureFromSurface(renderer, texte);
    SDL_FreeSurface(texte);
    

    position.x = x;
    position.y = y;
    if (largeur != 0) {
        position.w = largeur;
    }
    else {
        position.w = texte->w;
    }
    if (hauteur != 0) {
        position.h = hauteur;
    }
    else {
        position.h = texte->h;
    }

    int ok = SDL_RenderCopy(renderer, texte_texture, nullptr, &position);
   
    SDL_DestroyTexture(texte_texture);
    
}

void sdlFirstIa::sdlAffImgInHitBox(Image* img, hitBox* hitbox)
{
    int x = hitbox->x1;
    int y = hitbox->y1;
    int w = hitbox->getLargeur();
    int h = hitbox->getHauteur();
    img->draw(img->renderer, x, y, w, h);

}
