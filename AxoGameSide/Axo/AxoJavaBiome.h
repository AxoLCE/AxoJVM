#pragma once
#include "../Biome.h"

class AxoJavaBiome : public Biome {
private:
        int m_rawGrassColor;
        int m_rawFoliageColor;
        int m_rawWaterColor;
        int m_rawSkyColor;
public:
        AxoJavaBiome(int id, const wstring& name, int grassColor, int foliageColor, int waterColor, int skyColor, float temperature, float downfall, float depth, float scale, int topBlockId, int fillerBlockId);
        virtual int getGrassColor() const override;
        virtual int getFolageColor() const override;
        virtual int getWaterColor() override;
        virtual int getSkyColor(float temp) override;
};