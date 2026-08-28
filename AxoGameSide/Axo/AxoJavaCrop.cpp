#include "AxoJavaCrop.h"
#include "../Material.h"
#include "../Level.h"
#include "../Player.h"
#include "../ItemInstance.h"
#include "../IconRegister.h"
#include "../Inventory.h"
#include "../Tile.h"
#include "../AABB.h"

AxoJavaCrop::AxoJavaCrop(int id, const std::vector<wstring>& stageIconNames, int seedItemId, int dropItemId) : Tile(id, Material::plant, false) {
	m_stageIconNames = stageIconNames;
	m_seedItemId = seedItemId;
	m_dropItemId = dropItemId;

	setDestroyTime(0.0f);
	setSoundType(Tile::SOUND_GRASS);
	_isTicking = true;
}

void AxoJavaCrop::registerIcons(IconRegister *iconRegister) {
    m_stageIcons.clear();
    for (const auto& name : m_stageIconNames) {
        m_stageIcons.push_back(iconRegister->registerIcon(name));
    }
    if (!m_stageIcons.empty()) {
        icon = m_stageIcons[0];
    }
}
Icon *AxoJavaCrop::getTexture(int face, int data) {
    if (m_stageIcons.empty()) return nullptr;
    if (data < 0) data = 0;
    if (data >= (int)m_stageIcons.size()) data = m_stageIcons.size() - 1;
    return m_stageIcons[data];
}

int AxoJavaCrop::getResource(int data, Random* random, int playerBonusLevel) {
	if (data >= 7){
		return m_dropItemId;
	}
	return m_seedItemId;
}

void AxoJavaCrop::tick(Level *level, int x, int y, int z, Random *random) {
    int data = level->getData(x, y, z);
    
    if (data < 7) {
        int light = level->getRawBrightness(x, y + 1, z);
        if (light >= 9 && random->nextInt(10) == 0) {
            level->setData(x, y, z, data + 1, 3);
        }
    }
}

bool AxoJavaCrop::use(Level* level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly) {
	if (soundOnly) return false;
	shared_ptr<ItemInstance> item = player->getSelectedItem();
	if (item && item->id == 351) {
		int data = level->getData(x, y, z);
		if (data < 7) {
			level->setData(x, y, z, data + 1, 3);
			Tile *tile = id > 0 && id < Tile::TILE_NUM_COUNT ? Tile::tiles[id] : nullptr;
			for (int i = 0; i < 10; i++)
	            {
                     double xa = level->random->nextGaussian() * 0.02;
                     double ya = level->random->nextGaussian() * 0.02;
                     double za = level->random->nextGaussian() * 0.02;
                     level->addParticle(eParticleType_happyVillager, x + level->random->nextFloat(), y + level->random->nextFloat() * tile->getShapeY1(), z + level->random->nextFloat(), xa, ya, za);
                }
			if (!player->abilities.instabuild) {
				item->count--;
				if (item->count <= 0) {
					player->inventory->setItem(player->inventory->selected, nullptr);
				}
			}
			return true;
		}
	}
	return false;
}
void AxoJavaCrop::neighborChanged(Level *level, int x, int y, int z, int type) {
    if (level->getTile(x, y - 1, z) != Tile::farmland_Id) {
        int data = level->getData(x, y, z);
        spawnResources(level, x, y, z, data, 1.0f, 0);
        level->setTileAndUpdate(x, y, z, 0);
    }
}

void AxoJavaCrop::updateDefaultShape() {
    float ss = 0.5f;
    this->setShape(0.5f - ss, 0.0f, 0.5f - ss, 0.5f + ss, 0.25f, 0.5f + ss);
}
