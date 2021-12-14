#ifndef _SDL_Image_
#define _SDL_Image_

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


#include <string>

//! \brief Pour gerer une image avec SDL2
class Image {

public:
    Image();
    ~Image();

    void loadFromFile(const char* filename, SDL_Renderer* renderer);
    void loadFromCurrentSurface(SDL_Renderer* renderer);
    void draw(SDL_Renderer* renderer, const int& x, const int& y, const int& w = -1, const int& h = -1);
    SDL_Texture* getTexture() const;
    void setSurface(SDL_Surface* surf);
    SDL_Renderer* renderer;

private:

    SDL_Surface* surface;
    SDL_Texture* texture;
    bool has_changed;
    char name[100];

};
#endif