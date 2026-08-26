package axo.jvm.api;

public class ItemProperties {
    public String name = "";
    public String iconName = "";
    public int maxStackSize = 64;

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
}
