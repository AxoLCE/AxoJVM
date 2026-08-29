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

    public static void register(Biome biome, String modId){
        biome.assignRegistry(modId);
        BY_ID.put(biome.getId(), biome);
        ALL.add(biome);
    }

    public static void registerAllToNative(){
        for (Biome biome : ALL){
            int topBlockId = ItemMap.getItemIdByName(biome.getTopBlock());
            int fillerBlockId = ItemMap.getItemIdByName(biome.getFillerBlock());

            Bridge.registerBiome(
                    biome.getId(),
                    biome.getName(),
                    biome.getGrassColor(),
                    biome.getFoliageColor(),
                    biome.getTemperature(),
                    biome.getDownfall(),
                    topBlockId,
                    fillerBlockId
            );
        }
    }
}
