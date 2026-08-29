#include "AxoJavaBiome.h"
#include "../Tile.h"

AxoJavaBiome::AxoJavaBiome(int id, const std::wstring& name, int grassColor, int foliageColor, float temperature, float downfall, int topBlockId, int fillerBlockId) : Biome(id) {
    this->m_name = name;
    this->m_grassColor = (eMinecraftColour)grassColor;
    this->m_foliageColor = (eMinecraftColour)foliageColor;
    this->temperature = temperature;
    this->downfall = downfall;
    if (topBlockId != -1 && topBlockId >= 0 && topBlockId < 256) {
        this->topMaterial = (byte)topBlockId;
    }
    if (fillerBlockId != -1 && fillerBlockId >= 0 && fillerBlockId < 256) {
        this->material = (byte)fillerBlockId;
    }
}