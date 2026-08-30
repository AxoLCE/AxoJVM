#include "AxoJavaBiome.h"
#include "../Tile.h"

AxoJavaBiome::AxoJavaBiome(int id, const std::wstring& name, int grassColor, int foliageColor, int waterColor, int skyColor, float temperature, float downfall, float depth, float scale, int topBlockId, int fillerBlockId) : Biome(id) {
    this->m_name = name;
    this->m_rawGrassColor = grassColor;
    this->m_rawFoliageColor = foliageColor;
    this->m_rawWaterColor = waterColor;
    this->m_rawSkyColor = skyColor;
    this->temperature = temperature;
    this->downfall = downfall;
    this->depth = depth;
    this->scale = scale;
    if (topBlockId != -1 && topBlockId >= 0 && topBlockId < 256) {
        this->topMaterial = (byte)topBlockId;
    }
    if (fillerBlockId != -1 && fillerBlockId >= 0 && fillerBlockId < 256) {
        this->material = (byte)fillerBlockId;
    }
}

int AxoJavaBiome::getGrassColor() const {
    return m_rawGrassColor;
}
int AxoJavaBiome::getFolageColor() const {
    return m_rawFoliageColor;
}
int AxoJavaBiome::getWaterColor() {
    return m_rawWaterColor;
}
int AxoJavaBiome::getSkyColor(float temp) {
    return m_rawSkyColor;
}