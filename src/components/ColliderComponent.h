#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>
#include  "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include "./SpriteComponent.h"

class ColliderComponent: public Component {
    public:
        std::string colliderTag;
        SDL_Rect collider;
        SDL_Rect sourceRect;
        SDL_Rect destinationRect;
        TransformComponent* transform;

        ColliderComponent(std::string colliderTag, int x, int y, int width, int height) {
            this->colliderTag = colliderTag;
            this->collider = {x, y, width, height};
        }

        void Initialize() override {
            if (owner->HasComponent<TransformComponent>()) {
                transform = owner->GetComponent<TransformComponent>();
                sourceRect = {0, 0, transform->width, transform->height};
                destinationRect = {collider.x, collider.y, collider.w, collider.h};
            }
            
            owner->AddComponent<SpriteComponent>("collision-texture", 1, 1, false, false);
        }

        void Update(float deltaTime) override {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
            destinationRect.x = collider.x - Game::camera.x;
            destinationRect.y = collider.y - Game::camera.y;
        }
};

#endif
