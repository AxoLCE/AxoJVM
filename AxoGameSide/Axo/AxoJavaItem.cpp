#include "AxoJavaItem.h"
#include "../Tile.h"
#include"../Inventory.h"

AxoJavaItem::AxoJavaItem(int id, const wstring& iconName, const wstring& displayName, int maxStackSize, int plantBlockId) : Item(id) {
	m_javaIconName = iconName;
	m_displayName = displayName;
    m_plantBlockId = plantBlockId;
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