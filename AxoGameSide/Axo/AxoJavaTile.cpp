#include "AxoJavaTile.h"
#include "../IconRegister.h"
#include "../Item.h"

AxoJavaTile::AxoJavaTile(int id, Material* material, bool isSolidRender, float destroyTime, float explosionResistance, SoundType* soundType, const wstring& iconName, const wstring& displayName, int dropItemId) : Tile(id, material, isSolidRender) {
    
    m_javaIconName = iconName;
    m_displayName = displayName;
    m_dropItemId = dropItemId;
    setDestroyTime(destroyTime);
    setSoundType(soundType);
    setIconName(iconName);

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