#include <fstream>
#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"
#include "./components/TileComponent.h"

extern EntityManager manager;

Map::Map(std::string textureId, int scale, int tileSize) {
    this->textureId = textureId;
    this->scale = scale;
    this->tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY) {
    std::fstream mapFile;
    mapFile.open(filePath);

    for (int y = 0; y < mapSizeY; y++) {
        for (int x = 0; x < mapSizeX; x++) {
            char ch;
            mapFile.get(ch);
            int sourceY = atoi(&ch) * tileSize;
            mapFile.get(ch);
            int sourceX = atoi(&ch) * tileSize;
            AddTile(sourceX, sourceY, x * (scale * tileSize), y * (scale * tileSize));
            mapFile.ignore();
        }
    }
    mapFile.close();
}

void Map::AddTile(int sourceX, int sourceY, int x, int y) {
    Entity &newTile(manager.AddEntity("Tile"));
    newTile.AddComponent<TileComponent>(sourceX, sourceY, x, y, tileSize, scale, textureId);
}