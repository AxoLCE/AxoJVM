#pragma once
#include "../Item.h"
#include "../IconRegister.h"
#include "../Player.h"
#include "../Level.h"
#include "../ItemInstance.h"

class AxoJavaItem : public Item {
private:
	wstring m_javaIconName;
	wstring m_displayName;
	wstring m_registryName;
	int m_plantBlockId;
public:
	AxoJavaItem(int id, const wstring& iconName, const wstring& displayName, int maxStackSize, int plantBlockId, const wstring& registryName);
	virtual void registerIcons(IconRegister* iconRegister) override;
	virtual wstring getHoverName(shared_ptr<ItemInstance> itemInstance) override;
	virtual bool useOn(shared_ptr<ItemInstance> instance, shared_ptr<Player> player, Level* level, int x, int y, int z, int face, float clickX, float clickY, float clickZ, bool bTestUseOnOnly = false) override;
};