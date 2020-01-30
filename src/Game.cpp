#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./components/TransformComponent.h"
#include "./components/SpriteComponent.h"
#include "./components/KeyboardControlComponent.h"
#include "./components/ColliderComponent.h"
#include "./components/TextLabelComponent.h"
#include "./components/ProjectileEmitterComponent.h"
#include "../lib/glm/glm.hpp"
#include "./Map.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map *map;

Game::Game() {
    this->isRunning = false;
}

Game::~Game() {
}

bool Game::IsRunning() const {
    return this->isRunning;
}

void Game::Initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
        return;        
    }

    window = SDL_CreateWindow(
        "2D Game Engines with C++ SDL and Lua",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        std::cerr << "Error creating SDL window" << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(
        window, -1, 0
    );

    if (!renderer) {
        std::cerr << "Error rendering SDL window" << std::endl;
        return;
    }

    LoadLevel(0);

    isRunning = true;

    return;
}

Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber) {
    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());    
    assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());    
    assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());    
    assetManager->AddTexture("collision-texture", std::string("./assets/images/collision-texture.png").c_str());    
    assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());    
    assetManager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());
    assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);

    map = new Map("jungle-tiletexture", 2, 32);
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

    player.AddComponent<TransformComponent>(200, 321, 0, 0, 32, 32, 1);
    player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    player.AddComponent<KeyboardControlComponent>("up", "down", "left", "right", "space");
    player.AddComponent<ColliderComponent>("PLAYER", 200, 321, 32, 32);

    Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
    tankEntity.AddComponent<TransformComponent>(150, 495, 5, 0, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");
    tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);

    Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
    projectile.AddComponent<TransformComponent>(150+16, 495+16, 0, 0, 4, 4, 1);
    projectile.AddComponent<SpriteComponent>("projectile-image");
    projectile.AddComponent<ColliderComponent>("PROJECTILE", 150+16, 495+16,4, 4);
    projectile.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true);

    Entity& radarEntity(manager.AddEntity("radar", UI_LAYER));
    radarEntity.AddComponent<TransformComponent>(720, 20, 0, 0, 64, 64, 1);
    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

    Entity& heliport(manager.AddEntity("heliport", OBSTACLE_LAYER));
    heliport.AddComponent<TransformComponent>(420, 420, 0, 0, 32, 32, 1);
    heliport.AddComponent<SpriteComponent>("heliport-image");
    heliport.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);

    Entity& labelLvlName(manager.AddEntity("LabelLvlName", UI_LAYER));
    labelLvlName.AddComponent<TextLabelComponent>(10, 10, "First level", "charriot-font", WHITE_COLOR);

    manager.ListAllEntities();
}

void Game::ProcessInput() {
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            break;
        }
        default : {
            break;
        }
    }
}

void Game::Update() {

    //wait if the rendering is too fast
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

    //difference in ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    //if to slow limit the max deltatime
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    ticksLastFrame = SDL_GetTicks();

    manager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if (manager.HasNoEntities()) {
        return;
    }

    manager.Render();

    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::HandleCameraMovement() {
    TransformComponent* mainPlayerTransform = player.GetComponent<TransformComponent>();
    camera.x =  mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;

    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
    CollisionType collisionType = manager.CheckEntityCollisions();

    if (collisionType == PLAYER_ENEMY_COLLISION) {
        ProcessGameOver();
    }

    if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
        ProcessNextLevel(1);
    }    

    if (collisionType == PLAYER_PROJECTILE_COLLISION) {
        ProcessGameOver();
    }
}

void Game::ProcessNextLevel(int levelNumber) {
    std::cout << "Next level" << std::endl;
    isRunning = false;
}

void Game::ProcessGameOver() {
    std::cout << "Game Over" << std::endl;
    isRunning = false;
}