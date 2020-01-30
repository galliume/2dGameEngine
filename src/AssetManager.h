#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include "./TextureManager.h"
#include "./FontManager.h"
#include "./EntityManager.h"
#include <SDL2/SDL_ttf.h>

class AssetManager {
    private:
        EntityManager* manager;
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;
    public:
        AssetManager(EntityManager* manager);
        ~AssetManager();
        void ClearData();
        void AddTexture(std::string textureId, const char* fileName);
        SDL_Texture* GetTexture(std::string textureId);
        void AddFont(std::string fontId, const char* fileName, int fontSize);
        TTF_Font* GetFont(std::string fontId);        
};

#endif