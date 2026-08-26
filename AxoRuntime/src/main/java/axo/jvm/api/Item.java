package axo.jvm.api;

import axo.jvm.helpers.IdMap;

public class Item {
    private int id =-1;
    private String modId;
    private String registryName;
    protected final ItemProperties properties;

    public Item(ItemProperties props){
        this.properties = props;
        if(props.name.isEmpty()){
            throw new IllegalArgumentException("Item name is missing");
        }
    }

    public int getId(){
        return id;
    }
    public String getModId(){
        return modId;
    }
    public String getRegistryName(){
        return registryName;
    }
    public String getName(){
        return properties.name;
    }
    public String getIconName(){
        return properties.iconName;
    }
    public int getMaxStackSize(){
        return properties.maxStackSize;
    }

    public void assignRegistry(String modId){
        if (this.id != -1){
            throw new IllegalStateException("Item already registred");
        }
        this.modId = modId;
        this.registryName = modId + ":" + properties.name;
        this.id = IdMap.getOrAllocateItemId(modId, properties.name);
        if (properties.iconName.isEmpty()){
            properties.iconName = "axo:" + modId + ":" + properties.name;
        } else if (!properties.iconName.startsWith("axo:")){
            properties.iconName = "axo:" + modId + ":" + properties.iconName;
        }
    }

    public boolean onUse(long playerPtr, int x, int y, int z, int side){
        return false;
    }
}
