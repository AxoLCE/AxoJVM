package axo.jvm.api;

import axo.jvm.BlockRegistry;
import axo.jvm.helpers.IdMap;

public class Block {
    private int id = -1;
    private String modId;
    private String registryName;

    protected final BlockProperties properties;
    public Block(BlockProperties props){
        this.properties = props;
        if (props.name.isEmpty()){
            throw new IllegalArgumentException("Block name is missing");
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
    public String getMaterial(){
        return properties.material;
    }
    public float getDestroyTime(){
        return properties.destroyTime;
    }
    public float getExplosionResistance(){
        return properties.explosionResistance;
    }
    public String getSoundType(){
        return properties.soundType;
    }
    public String getIconName(){
        return properties.iconName;
    }
    public boolean isSolidRender(){
        return properties.isSolidRender;
    }
    public int getDropItemId(){
        return properties.dropItemId;
    }

    public void assignRegistry(String modId){
        if (this.id != -1){
            throw new IllegalStateException("Block already registred: " + registryName);
        }
        this.modId = modId;
        this.registryName = modId + ":" + properties.name;
        this.id = IdMap.getOrAllocateTileId(modId, properties.name);
        if (properties.iconName.isEmpty()){
            properties.iconName = "axo:" + modId + ":" + properties.name;
        } else if (!properties.iconName.startsWith("axo:")){
            properties.iconName = "axo:" + modId + ":" + properties.iconName;
        }
    }

    public BlockProperties getProperties(){
        return properties;
    }

    // FOR LATER!!
    public void onTick(long levelPtr, int x, int y, int z){}
    public void onPlace(long levelPtr, int x, int y, int z){}
    public void onBreak(long levelPtr, int x, int y, int z){};
    public boolean onUse(long levelPtr, int x, int y, int z){
        return false;
    }
    public void onNeighborChanged(long levelPtr, int x, int y, int z, int neighborId){}
}
