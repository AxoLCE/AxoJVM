package axo.jvm.api;

public class ItemProperties {
    public static final int USE_ANIMATION_NONE = 0;
    public static final int USE_ANIMATION_EAT = 1;
    public static final int USE_ANIMATION_DRINK = 2;
    public static final int USE_ANIMATION_BLOCK = 3;
    public static final int USE_ANIMATION_BOW = 4;

    public static final int RARITY_COMMON = 0;
    public static final int RARITY_UNCOMMON = 1;
    public static final int RARITY_RARE = 2;
    public static final int RARITY_EPIC = 3;

    public String name = "";
    public String iconName = "";
    public int maxStackSize = 64;
    public int maxDamage = 0;
    public float attackDamage = 0.0f;
    public float destroySpeed = 1.0f;
    public int enchantmentValue = 0;
    public boolean handEquipped = false;
    public boolean stackedByData = false;
    public int useAnimation = USE_ANIMATION_NONE;
    public int useDuration = 0;
    public boolean foil = false;
    public int rarity = RARITY_COMMON;
    public int plantBlockId = -1;

    public static ItemProperties of(){
        return new ItemProperties();
    }
    public ItemProperties name(String s){
        this.name = s;
        return this;
    }
    public ItemProperties iconName(String s){
        this.iconName = s;
        return this;
    }
    public ItemProperties maxStackSize (int i){
        this.maxStackSize = i;
        return this;
    }
    public ItemProperties maxDamage(int i){
        this.maxDamage = i;
        return this;
    }
    public ItemProperties attackDamage(float f){
        this.attackDamage = f;
        return this;
    }
    public ItemProperties destroySpeed(float f){
        this.destroySpeed = f;
        return this;
    }
    public ItemProperties enchantmentValue(int i){
        this.enchantmentValue = i;
        return this;
    }
    public ItemProperties handEquipped(boolean b){
        this.handEquipped = b;
        return this;
    }
    public ItemProperties stackedByData(boolean b){
        this.stackedByData = b;
        return this;
    }
    public ItemProperties useAnimation(int i){
        this.useAnimation = i;
        return this;
    }
    public ItemProperties useDuration(int i){
        this.useDuration = i;
        return this;
    }
    public ItemProperties foil(boolean b){
        this.foil = b;
        return this;
    }
    public ItemProperties rarity(int i){
        this.rarity = i;
        return this;
    }
    public ItemProperties plantBlock(Block block){
        this.plantBlockId = block.getId();
        return this;
    }
}
