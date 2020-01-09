#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "./TransformComponent.h"

class SpriteComponent: public Component {
    private:
        SDL_Texture* texture;
        SDL_Rect sourceRectangle;
        SDL_Rect destinationRectangle;
        TransformComponent* transform;

    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

        SpriteComponent(const char* fileName) {
            SetTexture(fileName);
        }

        void SetTexture(std::string assetTextureId) {
            texture = Game::assetManager->GetTexture(assetTextureId);
        }

        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();
            sourceRectangle.x = 0;
            sourceRectangle.y = 0;
            sourceRectangle.w = transform->width;
            sourceRectangle.h = transform->height;
        }

        void Update(float deltaTime) override {
            destinationRectangle.x = (int) transform->position.x;
            destinationRectangle.y = (int) transform->position.y;
            destinationRectangle.w = (int) transform->width * transform->scale;
            destinationRectangle.h = transform->height * transform->scale;
        }

        void Render() override {
            TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
        }
};

#endif
