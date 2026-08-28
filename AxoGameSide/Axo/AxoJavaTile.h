#pragma once
#include "../Tile.h"

class AxoJavaTile : public Tile {
private:
	wstring m_javaIconName;
	wstring m_displayName;
	int m_dropItemId;
	int m_renderShapeId;
	bool m_canWalkThrough;
public:
	AxoJavaTile(int id, Material* material, bool isSolidRender, float destroyTime, float explosionResistance, SoundType* soundType, const wstring& iconName, const wstring& displayName, int dropItemId, int renderShapeId, bool canWalkThrough);
	virtual void registerIcons(IconRegister* iconRegister) override;
	virtual int getRenderShape() override {
		return m_renderShapeId;
	}
	virtual wstring getName() {
		return m_displayName;
	}
	virtual int getResource(int data, Random* random, int playerBonusLevel) override;
	virtual AABB *getAABB(Level *level, int x, int y, int z) override;
    virtual bool isSolidRender(bool isServerLevel = false) override;
    virtual bool isCubeShaped() override;
    virtual bool blocksLight() override;
};