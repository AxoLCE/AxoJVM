#include "AxoJavaTile.h"
#include "../IconRegister.h"

AxoJavaTile::AxoJavaTile(int id, Material* material, bool isSolidRender, float destroyTime, float explosionResistance, SoundType* soundType, const wstring& iconName, const wstring& displayName) : Tile(id, material, isSolidRender) {
    
    m_javaIconName = iconName;
    m_displayName = displayName;
    setDestroyTime(destroyTime);
    setSoundType(soundType);
    setIconName(iconName);
}
// HARDCODED TEXTURE FOR LATER CHANGE!!!
void AxoJavaTile::registerIcons(IconRegister* iconRegister) {
     if (m_javaIconName.rfind(L"axo:", 0) == 0) {
		icon = iconRegister->registerIcon(L"stone");
		return;
	}
	icon = iconRegister->registerIcon(m_javaIconName);
}