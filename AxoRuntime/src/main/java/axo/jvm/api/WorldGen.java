package axo.jvm.api;

import axo.jvm.BiomeRegistry;
import axo.jvm.Bridge;
import axo.jvm.helpers.ItemMap;

import java.util.HashSet;
import java.util.Set;

public abstract class WorldGen {
    public enum Phase {
        SURFACE, DECORATE
    }

    private Phase phase = Phase.DECORATE;
    private final Set<String> biomes = new HashSet<>();
    private int dimension = Dimension.OVERWORLD.id;

    protected final void setPhase(Phase phase){
        this.phase = phase;
    }

    protected final void setBiomes(String... registryName){
        biomes.clear();
        for (String name : registryName){
            biomes.add(name.contains(":") ? name : "minecraft:" + name);
        }
    }

    protected final void setDimension(Dimension dimension){
        this.dimension = dimension.id;
    }

    public Phase getPhase(){
        return phase;
    }

    public int getDimension(){
        return dimension;
    }

    public boolean appliesToBiome(int biomeId){
        if(biomes.isEmpty()){
            return true;
        }
        String name = BiomeRegistry.getNameForId(biomeId);
        return name != null && biomes.contains(name);
    }

    public boolean appliesToDimension(int dimension){
        return this.dimension == dimension;
    }

    public abstract void generate(long levelPtr, int chunkX, int chunkZ, long randomPtr);

    protected static int randomInt(long randomPtr, int bound) {
        return Bridge.randomInt(randomPtr, bound);
    }
    protected static void setRandomSeed(long randomPtr, long seed) {
        Bridge.setRandomSeed(randomPtr, seed);
    }
    protected static int getBlock(long levelPtr, int x, int y, int z) {
        return Bridge.getBlock(levelPtr, x, y, z);
    }
    protected static void setBlock(long levelPtr, int x, int y, int z, int tile) {
        Bridge.setBlock(levelPtr, x, y, z, tile);
    }
    protected static void setBlockWithData(long levelPtr, int x, int y, int z, int tile, int data) {
        Bridge.setBlockWithData(levelPtr, x, y, z, tile, data);
    }
    protected static int getBiomeAt(long levelPtr, int x, int z) {
        return Bridge.getBiomeAt(levelPtr, x, z);
    }
    protected static int getSeaLevel(long levelPtr) {
        return Bridge.getSeaLevel(levelPtr);
    }
}
