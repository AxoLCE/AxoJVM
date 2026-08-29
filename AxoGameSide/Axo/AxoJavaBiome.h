#pragma once
#include "../Biome.h"

class AxoJavaBiome : public Biome {
public:
	AxoJavaBiome(int id, const wstring& name, int grassColor, int foliageColor, float temperature, float downfall, int topBlockId, int fillerBlockId);
};