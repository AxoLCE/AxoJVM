#pragma once
#include "../Tile.h"

class AxoJavaTile : public Tile {
private:
	wstring m_javaIconName;
	wstring m_displayName;
	int m_dropItemId;
public:
	AxoJavaTile(int id, Material* material, bool isSolidRender, float destroyTime, float explosionResistance, SoundType* soundType, const wstring& iconName, const wstring& displayName, int dropItemId);
	virtual void registerIcons(IconRegister* iconRegister) override;
	virtual int getRenderShape() override {
		return SHAPE_BLOCK;
	}
	virtual wstring getName() {
		return m_displayName;
	}
	virtual int getResource(int data, Random* random, int playerBonusLevel) override;
};