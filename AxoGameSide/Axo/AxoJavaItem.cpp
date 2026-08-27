#include "AxoJavaItem.h"

AxoJavaItem::AxoJavaItem(int id, const wstring& iconName, const wstring& displayName, int maxStackSize) : Item(id) {
	m_javaIconName = iconName;
	m_displayName = displayName;
	setMaxStackSize(maxStackSize);
	setIconName(iconName);
}
// Not hardcoded anymore
void AxoJavaItem::registerIcons(IconRegister* iconRegister) {
	icon = iconRegister->registerIcon(m_javaIconName);
}