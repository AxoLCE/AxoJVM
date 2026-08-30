package axo.jvm;

import axo.jvm.api.Biome;
import axo.jvm.helpers.ItemMap;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class BiomeRegistry {
    private static final Map<Integer, Biome> BY_ID = new HashMap<>();
    private static final List<Biome> ALL = new ArrayList<>();
    private static final Map<Integer, String> VANILLA_BY_ID = new HashMap<>();

    static {
        VANILLA_BY_ID.put(0, "minecraft:ocean");
        VANILLA_BY_ID.put(1, "minecraft:plains");
        VANILLA_BY_ID.put(2, "minecraft:desert");
        VANILLA_BY_ID.put(3, "minecraft:extreme_hills");
        VANILLA_BY_ID.put(4, "minecraft:forest");
        VANILLA_BY_ID.put(5, "minecraft:taiga");
        VANILLA_BY_ID.put(6, "minecraft:swampland");
        VANILLA_BY_ID.put(7, "minecraft:river");
        VANILLA_BY_ID.put(12, "minecraft:ice_flats");
        VANILLA_BY_ID.put(16, "minecraft:beaches");
        VANILLA_BY_ID.put(21, "minecraft:jungle");
        VANILLA_BY_ID.put(27, "minecraft:birch_forest");
        VANILLA_BY_ID.put(29, "minecraft:roofed_forest");
        VANILLA_BY_ID.put(30, "minecraft:cold_taiga");
        VANILLA_BY_ID.put(32, "minecraft:mega_taiga");
        VANILLA_BY_ID.put(35, "minecraft:savanna");
        VANILLA_BY_ID.put(37, "minecraft:mesa");
    }

    public static void register(Biome biome, String modId){
        biome.assignRegistry(modId);
        BY_ID.put(biome.getId(), biome);
        ALL.add(biome);
    }

    public static void registerAllToNative() {
        for (Biome biome : ALL) {
            int topBlockId = ItemMap.getItemIdByName(biome.getTopBlock());
            int fillerBlockId = ItemMap.getItemIdByName(biome.getFillerBlock());

            Bridge.registerBiome(
                    biome.getId(),
                    biome.getName(),
                    biome.getGrassColor(),
                    biome.getFoliageColor(),
                    biome.getWaterColor(),
                    biome.getSkyColor(),
                    biome.getTemperature(),
                    biome.getDownfall(),
                    biome.getDepth(),
                    biome.getScale(),
                    topBlockId,
                    fillerBlockId
            );
            if (biome.getSpawnWeight() > 0 && biome.getCategory() != null) {
                Bridge.registerBiomeSpawn(biome.getId(), biome.getCategory().id, biome.getSpawnWeight());
            }
        }
    }
    public static Biome get(int id) {
        return BY_ID.get(id);
    }

    public static String getNameForId(int id) {
        Biome biome = BY_ID.get(id);
        if (biome != null) return biome.getRegistryName();
        return VANILLA_BY_ID.get(id);
    }
}
