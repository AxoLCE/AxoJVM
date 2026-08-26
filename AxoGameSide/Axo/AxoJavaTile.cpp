#include "AxoJavaTile.h"
#include "../IconRegister.h"

AxoJavaTile::AxoJavaTile(int id, Material* material, bool isSolidRender, float destroyTime, float explosionResistance, SoundType* soundType, const wstring& iconName, const wstring& displayName) : Tile(id, material, isSolidRender) {
    
    m_javaIconName = iconName;
    m_displayName = displayName;
    setDestroyTime(destroyTime);
    setSoundType(soundType);
    setIconName(iconName);
}

void AxoJavaTile::registerIcons(IconRegister* iconRegister) {
     icon = iconRegister->registerIcon(L"stone");
    }