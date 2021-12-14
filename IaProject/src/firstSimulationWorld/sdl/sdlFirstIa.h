#pragma once

#include <iostream>
#include <map>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "../simulation/simulation.h"
#include "HitBox.h"
#include "Image.h"

using namespace std;

class sdlFirstIa
{
public:
    sdlFirstIa(unsigned int x, unsigned int y,unsigned int zoom, float atioNBIA, float proportionNourriture);
    ~sdlFirstIa();
    /**
   \brief Initialise SDL
   \details Initialise sdl, charge la police, cree la fenetre et le rendu
   */
    void init_SDL();
    
    /**\brief Appel les differentes fonctions d'affichage pour creer le render qui sera afficher par SDL_RenderPresent(SDL_Renderer*);
    */
    void sdlAff();

    void sdlAffTerrain();

    void sdlAffTexte(const string& txt, const int& x, const int& y, const int& largeur, const int& hauteur, const SDL_Color& couleur, SDL_Renderer* renderer);

    void sdlAffImgInHitBox(Image* img, hitBox* hitbox);

    void boucle();

    void entityBrainLoop();

    void initSimu();

    void affEntiteData();

    void affEntiteBrain();

    void getNeuronDependance(vector<neurons*>* inputsNeurons, vector<neurons*>* list, vector<neurons*>* direct, vector<neurons*>* reste);

    Image& getImage(string path, SDL_Renderer*);

protected:
    bool pause;
    bool pauseAff;
    bool affBrain;
    bool affInfo;
    std::map<unsigned long, int[3]>  NeuronsPosition;

    //------------------Donn�es SDL
    SDL_Window* window;
    SDL_Window* windowEntiteData;
    SDL_Window* windowEntiteBrain;

    SDL_Renderer* renderer;
    SDL_Renderer* rendererEntiteData;
    SDL_Renderer* rendererEntiteBrain;

    SDL_Event event;
    SDL_Color font_color;
    TTF_Font* font;

    void setFontSize(int size);
    simulation simu;
    unsigned int dimWinX;
    unsigned int dimWinY;
    unsigned int zoom;

    
    map<string, Image> im_entite;
    vector<hitBoxEntite> entityHitbox;
    map<unsigned long, hitBoxNeurons> neuronsAffHitbox;
    unsigned long idSelectedNeuron;
    unsigned long trackedEntityId;
    unsigned int tempo;
    int stepToDo;

};

