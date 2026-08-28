#include "AxoJavaTile.h"
#include "../IconRegister.h"
#include "../Item.h"

AxoJavaTile::AxoJavaTile(int id, Material* material, bool isSolidRender, float destroyTime, float explosionResistance, SoundType* soundType, const wstring& iconName, const wstring& displayName, int dropItemId, int renderShape, bool canWalkThrough) : Tile(id, material, isSolidRender) {
    
    m_javaIconName = iconName;
    m_displayName = displayName;
    m_dropItemId = dropItemId;
    m_renderShapeId = renderShape;
    m_canWalkThrough = canWalkThrough;
    setDestroyTime(destroyTime);
    setSoundType(soundType);
    setIconName(iconName);
    setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

}
// not hardcoded anymore
void AxoJavaTile::registerIcons(IconRegister* iconRegister) {
	icon = iconRegister->registerIcon(m_javaIconName);
}

// for drops
int AxoJavaTile::getResource(int data, Random* random, int playerBonusLevel) {
    if (m_dropItemId != -1) {
        return m_dropItemId;
    }
    return Tile::getResource(data, random, playerBonusLevel);
}

AABB *AxoJavaTile::getAABB(Level *level, int x, int y, int z) {
    if (m_canWalkThrough) {
        return nullptr;
    }
    return Tile::getAABB(level, x, y, z);
}

bool AxoJavaTile::isSolidRender(bool isServerLevel) {
    if (m_canWalkThrough) return false;
    return Tile::isSolidRender(isServerLevel);
}

bool AxoJavaTile::isCubeShaped() {
    if (m_canWalkThrough) return false;
    return Tile::isCubeShaped();
}

bool AxoJavaTile::blocksLight() {
    if (m_canWalkThrough) return false;
    return Tile::blocksLight();
}