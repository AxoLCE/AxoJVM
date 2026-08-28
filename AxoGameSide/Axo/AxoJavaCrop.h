#pragma once
#include "../Tile.h"
#include "../AABB.h"
#include "../IconRegister.h"
#include <vector>

class AxoJavaCrop : public Tile {
private:
	std::vector<wstring> m_stageIconNames;
	std::vector<Icon*> m_stageIcons;
	int m_seedItemId;
	int m_dropItemId;
public:
	AxoJavaCrop(int id, const std::vector<wstring>& stageIconNames, int seedItemId, int dropItemId);
	virtual int getRenderShape() override {
		return SHAPE_ROWS;
	}

	virtual Icon* getTexture(int face, int data) override;
	virtual void registerIcons(IconRegister* iconRegister) override;
	virtual int getResource(int data, Random* random, int playerBonusLevel) override;
	virtual bool use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly = false) override;
	
	virtual void tick(Level *level, int x, int y, int z, Random *random) override;
	
	virtual bool isSolidRender(bool isServerLevel = false) override {
		return false;
	}
    virtual bool isCubeShaped() override {
		return false;
	}
    virtual bool blocksLight() override {
		return false;
	}
    virtual void updateDefaultShape() override;
	virtual void neighborChanged(Level *level, int x, int y, int z, int type) override;
	virtual AABB *getAABB(Level *level, int x, int y, int z) override {
		return nullptr;
	}
};