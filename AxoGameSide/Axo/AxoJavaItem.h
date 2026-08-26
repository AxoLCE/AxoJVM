#pragma once
#include "../Item.h"
#include "../IconRegister.h"

class AxoJavaItem : public Item {
private:
	wstring m_javaIconName;
	wstring m_displayName;
public:
	AxoJavaItem(int id, const wstring& iconName, const wstring& displayName, int maxStackSize);
	virtual void registerIcons(IconRegister* iconRegister) override;
	virtual wstring getHoverName(shared_ptr<ItemInstance> itemInstance) override {
		return m_displayName;
	}
};