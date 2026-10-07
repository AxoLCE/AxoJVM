#include "AxoJavaItem.h"
#include "AxoBridge.h"
#include "../Tile.h"
#include "../Inventory.h"
#include "../Language.h"
#include "../net.minecraft.world.entity.h"
#include "../net.minecraft.world.entity.ai.attributes.h"
#include "../SharedMonsterAttributes.h"
#include "../AttributeModifier.h"


AxoJavaItem::AxoJavaItem(
    int id,
    const wstring& iconName,
    const wstring& displayName,
    int maxStackSize,
    int maxDamage,
    float attackDamage,
    float destroySpeed,
    int enchantmentValue,
    bool handEquipped,
    bool stackedByData,
    int useAnimation,
    int useDuration,
    bool foil,
    int rarity,
    int plantBlockId,
    const wstring& registryName
) : Item(id) {
    m_javaIconName = iconName;
    m_displayName = displayName;
    m_registryName = registryName;
    m_plantBlockId = plantBlockId;
    m_maxDamage = maxDamage;
    m_attackDamage = attackDamage;
    m_destroySpeed = destroySpeed;
    m_enchantmentValue = enchantmentValue;
    m_handEquipped = handEquipped;
    m_stackedByData = stackedByData;
    m_useAnimation = useAnimation;
    m_useDuration = useDuration;
    m_foil = foil;
    m_rarity = rarity;

    setMaxStackSize(maxStackSize);
    setMaxDamage(maxDamage);
    setStackedByData(stackedByData);
    setIconName(iconName);
}

// Not hardcoded anymore
void AxoJavaItem::registerIcons(IconRegister* iconRegister) {
    icon = iconRegister->registerIcon(m_javaIconName);
}

bool AxoJavaItem::useOn(
    shared_ptr<ItemInstance> instance,
    shared_ptr<Player> player,
    Level* level,
    int x,
    int y,
    int z,
    int face,
    float clickX,
    float clickY,
    float clickZ,
    bool bTestUseOnOnly
) {
    if (m_plantBlockId != -1 && face == 1) {
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
    return false;
}

float AxoJavaItem::getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile* tile) {
    return m_destroySpeed;
}

bool AxoJavaItem::hurtEnemy(
    shared_ptr<ItemInstance> itemInstance,
    shared_ptr<LivingEntity> mob,
    shared_ptr<LivingEntity> attacker
) {
    if (m_maxDamage > 0) {
        itemInstance->hurtAndBreak(1, attacker);
        return true;
    }
    return false;
}

bool AxoJavaItem::mineBlock(
    shared_ptr<ItemInstance> itemInstance,
    Level* level,
    int tile,
    int x,
    int y,
    int z,
    shared_ptr<LivingEntity> owner
) {
    if (m_maxDamage > 0) {
        if (Tile::tiles[tile]->getDestroySpeed(level, x, y, z) != 0.0) {
            itemInstance->hurtAndBreak(1, owner);
        }

        return true;
    }

    return false;
}

bool AxoJavaItem::isHandEquipped() {
    return m_handEquipped;
}

int AxoJavaItem::getEnchantmentValue() {
    return m_enchantmentValue;
}

UseAnim AxoJavaItem::getUseAnimation(shared_ptr<ItemInstance> itemInstance) {
    switch (m_useAnimation) {
    case 1:
        return UseAnim_eat;
    case 2:
        return UseAnim_drink;
    case 3:
        return UseAnim_block;
    case 4:
        return UseAnim_bow;
    default:
        return UseAnim_none;
    }
}

int AxoJavaItem::getUseDuration(shared_ptr<ItemInstance> itemInstance) {
    return m_useDuration;
}

shared_ptr<ItemInstance> AxoJavaItem::use(
    shared_ptr<ItemInstance> instance,
    Level* level,
    shared_ptr<Player> player
) {
    if (m_useDuration > 0) {
        player->startUsingItem(instance, m_useDuration);
    }
    return instance;
}

bool AxoJavaItem::isFoil(shared_ptr<ItemInstance> itemInstance) {
    return m_foil || Item::isFoil(itemInstance);
}

const Rarity* AxoJavaItem::getRarity(shared_ptr<ItemInstance> itemInstance) {
    if (itemInstance->isEnchanted()) {
        return Rarity::rare;
    }
    switch (m_rarity) {
    case 1:
        return Rarity::uncommon;
    case 2:
        return Rarity::rare;
    case 3:
        return Rarity::epic;
    default:
        return Rarity::common;
    }
}

bool AxoJavaItem::isEnchantable(shared_ptr<ItemInstance> itemInstance) {
    return m_enchantmentValue > 0 || Item::isEnchantable(itemInstance);
}

attrAttrModMap* AxoJavaItem::getDefaultAttributeModifiers() {
    attrAttrModMap* result = Item::getDefaultAttributeModifiers();
    if (m_attackDamage != 0.0f) {
        (*result)[SharedMonsterAttributes::ATTACK_DAMAGE->getId()] =
            new AttributeModifier(
                eModifierId_ITEM_BASEDAMAGE,
                m_attackDamage,
                AttributeModifier::OPERATION_ADDITION
            );
    }
    return result;
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