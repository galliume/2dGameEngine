#include <iostream>
#include "./EntityManager.h"
#include "./Collision.h"
#include "components/ColliderComponent.h"

void EntityManager::ClearData() {
    for(auto& entity: entities) {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities() {
    return entities.size() == 0;
}

void EntityManager::Update(float deltaTime) {
    for (auto& entity: entities) {
        entity->Update(deltaTime);
    }

    DestroyInactiveEntities();
}

void EntityManager::DestroyInactiveEntities() {
    for (int i = 0; i < entities.size(); i++) {
        if (!entities[i]->IsActive()) {
            entities.erase(entities.begin() + i);
        }
    }
}

void EntityManager::Render() {

    for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
        for (auto& entity: GetEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
            entity->Render();
        }
    }
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer) {
    Entity *entity = new Entity(*this, entityName, layer);
    entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
    return entities;
}

unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}

void EntityManager::ListAllEntities() const {
    unsigned int i = 0;
    for (auto& entity: entities) {
        std::cout << "Entity[" << i << "]: " << entity->name << std::endl;
        entity->ListAllComponents();
        i++;
    }
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> selectedEntities;
    for (auto& entity: entities) {
        if (entity->layer == layer) {
            selectedEntities.emplace_back(entity);
        }
    }

    return selectedEntities;
}

CollisionType EntityManager::CheckEntityCollisions() const {
    for (auto& thisEntity: entities) {
        if (thisEntity->HasComponent<ColliderComponent>()) {
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for (auto& thatEntity: entities) {
                if (thisEntity->name.compare(thatEntity->name) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if (Collision::CheckRectCollision(thisCollider->collider, thatCollider->collider)) {
                        if (
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("ENEMY") == 0
                        ) {
                            return PLAYER_ENEMY_COLLISION;
                        }

                        if (
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("PROJECTILE") == 0
                        ) {
                            return PLAYER_PROJECTILE_COLLISION;
                        }

                        if (
                            thisCollider->colliderTag.compare("ENEMY") == 0 &&
                            thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0
                        ) {
                            return PLAYER_PROJECTILE_COLLISION;
                        }

                        if (
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0
                        ) {
                            return PLAYER_LEVEL_COMPLETE_COLLISION;
                        }
                    }
                }
            }
        }
    }

    return NO_COLLISION;
}