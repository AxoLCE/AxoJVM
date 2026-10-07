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
    int m_maxDamage;
    float m_attackDamage;
    float m_destroySpeed;
    int m_enchantmentValue;
    bool m_handEquipped;
    bool m_stackedByData;
    int m_useAnimation;
    int m_useDuration;
    bool m_foil;
    int m_rarity;

public:
    AxoJavaItem(
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
    );

    virtual void registerIcons(IconRegister* iconRegister) override;
    virtual wstring getHoverName(shared_ptr<ItemInstance> itemInstance) override;
    virtual bool useOn(shared_ptr<ItemInstance> instance, shared_ptr<Player> player, Level* level, int x, int y, int z, int face, float clickX, float clickY, float clickZ, bool bTestUseOnOnly = false) override;
    virtual float getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile* tile) override;
    virtual bool hurtEnemy(shared_ptr<ItemInstance> itemInstance, shared_ptr<LivingEntity> mob, shared_ptr<LivingEntity> attacker) override;
    virtual bool mineBlock(shared_ptr<ItemInstance> itemInstance, Level* level, int tile, int x, int y, int z, shared_ptr<LivingEntity> owner) override;
    virtual bool isHandEquipped() override;
    virtual int getEnchantmentValue() override;
    virtual UseAnim getUseAnimation(shared_ptr<ItemInstance> itemInstance) override;
    virtual int getUseDuration(shared_ptr<ItemInstance> itemInstance) override;
    virtual shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> instance, Level* level, shared_ptr<Player> player) override;
    virtual bool isFoil(shared_ptr<ItemInstance> itemInstance) override;
    virtual const Rarity* getRarity(shared_ptr<ItemInstance> itemInstance) override;
    virtual bool isEnchantable(shared_ptr<ItemInstance> itemInstance) override;
    virtual attrAttrModMap* getDefaultAttributeModifiers() override;
};