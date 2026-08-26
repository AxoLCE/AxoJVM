package axo.jvm.api;

public class BlockProperties {
    public String name = "";
    public String material = "stone";
    public float destroyTime = 0.0f;
    public float explosionResistance = 0.0f;
    public String soundType = "STONE";
    public String iconName = "";
    public boolean isSolidRender = true;

    public static BlockProperties of(){
        return new BlockProperties();
    }
    public BlockProperties name(String s) {
        this.name = s;
        return this;
    }
    public BlockProperties material(String s) {
        this.material = s;
        return this;
    }
    public BlockProperties destroyTime(float f){
        this.destroyTime = f;
        return this;
    }
    public BlockProperties explosionResistance(float f){
        this.explosionResistance = f;
        return this;
    }
    public BlockProperties soundType(String s){
        this.soundType = s;
        return this;
    }
    public BlockProperties iconName(String s){
        this.iconName = s;
        return this;
    }
    public BlockProperties isSolidRender(boolean b){
        this.isSolidRender = b;
        return this;
    }
}
