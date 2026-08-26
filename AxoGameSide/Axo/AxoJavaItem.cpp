#include "AxoJavaItem.h"

AxoJavaItem::AxoJavaItem(int id, const wstring& iconName, const wstring& displayName, int maxStackSize) : Item(id) {
	m_javaIconName = iconName;
	m_displayName = displayName;
	setMaxStackSize(maxStackSize);
	setIconName(iconName);
}
// HARDCODED TEXTURE!!!
void AxoJavaItem::registerIcons(IconRegister* iconRegister) {
	if (m_javaIconName.rfind(L"axo:", 0) == 0) {
		icon = iconRegister->registerIcon(L"stick");
		return;
	}
	icon = iconRegister->registerIcon(m_javaIconName);
}