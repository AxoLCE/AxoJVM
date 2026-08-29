#include "AxoJavaItem.h"
#include "AxoBridge.h"
#include "../Tile.h"
#include"../Inventory.h"
#include "../Language.h"

AxoJavaItem::AxoJavaItem(int id, const wstring& iconName, const wstring& displayName, int maxStackSize, int plantBlockId, const wstring& registryName) : Item(id) {
	m_javaIconName = iconName;
	m_displayName = displayName;
    m_plantBlockId = plantBlockId;
    m_registryName = registryName;
	setMaxStackSize(maxStackSize);
	setIconName(iconName);
}
// Not hardcoded anymore
void AxoJavaItem::registerIcons(IconRegister* iconRegister) {
	icon = iconRegister->registerIcon(m_javaIconName);
}

bool AxoJavaItem::useOn(shared_ptr<ItemInstance> instance, shared_ptr<Player> player, Level* level, int x, int y, int z, int face, float clickX, float clickY, float clickZ, bool bTestUseOnOnly) {
    if (m_plantBlockId != -1) {
        if (face == 1) {
            int blockBelow = level->getTile(x, y, z);
 
            if (blockBelow == Tile::farmland_Id) {
                if (!bTestUseOnOnly) {
                    level->setTileAndUpdate(x, y + 1, z, m_plantBlockId);
                    
                    if (!player->abilities.instabuild) {
                        instance->count--;
                        if (instance->count <= 0) {
                            player->inventory->setItem(player->inventory->selected, nullptr);
                        }
                    }
                }
                return true;
            }
        }
    }
    return false;
}

wstring AxoJavaItem::getHoverName(shared_ptr<ItemInstance> itemInstance) {
    std::wstring wKey = L"item." + m_registryName + L".name";
    std::string key(wKey.begin(), wKey.end());
    std::wstring translated = AxoBridge_GetLang(key);
    if (translated != wKey) {
        return translated;
    }
    return m_displayName;
}
