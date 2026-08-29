package axo.jvm.api;

import axo.jvm.helpers.IdMap;

public class Biome {
    private int id = -1;
    private String modId;
    private String registryName;

    protected final BiomeProperties properties;
    public Biome(BiomeProperties props){
        this.properties = props;
        if (props.name.isEmpty()){
            throw new IllegalArgumentException("[AxoJVM] Biome name required");
        }
    }
    public int getId() {
        return id;
    }
    public String getRegistryName() {
        return registryName;
    }
    public String getName() {
        return properties.name;
    }
    public int getGrassColor() {
        return properties.grassColor;
    }
    public int getFoliageColor() {
        return properties.foliageColor;
    }
    public float getTemperature() {
        return properties.temperature;
    }
    public float getDownfall() {
        return properties.downfall;
    }
    public String getTopBlock() {
        return properties.topBlock;
    }
    public String getFillerBlock() {
        return properties.fillerBlock;
    }

    public void assignRegistry(String modId) {
        if (this.id != -1) throw new IllegalStateException("[AxoJVM] Biome already registered");
        this.modId = modId;
        this.registryName = modId + ":" + properties.name;
        this.id = IdMap.getOrAllocateBiomeId(modId, properties.name);
    }
}
