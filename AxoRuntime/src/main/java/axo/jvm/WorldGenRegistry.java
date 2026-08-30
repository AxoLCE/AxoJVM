package axo.jvm;

import axo.jvm.api.WorldGen;

import java.util.ArrayList;
import java.util.List;

public class WorldGenRegistry {
    private static final List<WorldGen> ALL = new ArrayList<>();

    public static void register(WorldGen gen, String modId){
        ALL.add(gen);
        System.out.println("[AxoJVM] Registered worldgen (phase " + gen.getPhase() + ", dimension " + gen.getDimension() + ") for mod " + modId);
    }

    public static void runSurface(long levelPtr, int chunkX, int chunkZ, long randomPtr, int biomeId, int dimension) {
        for (WorldGen gen : ALL) {
            if (gen.getPhase() != WorldGen.Phase.SURFACE) continue;
            if (!gen.appliesToDimension(dimension)) continue;
            if (!gen.appliesToBiome(biomeId)) continue;
            try {
                gen.generate(levelPtr, chunkX, chunkZ, randomPtr);
            } catch (Exception e) {
                System.out.println("[AxoJVM] ERROR: surface gen failed: " + e.getMessage());
            }
        }
    }

    public static void runDecorate(long levelPtr, int chunkX, int chunkZ, long randomPtr, int biomeId, int dimension) {
        for (WorldGen gen : ALL) {
            if (gen.getPhase() != WorldGen.Phase.DECORATE) continue;
            if (!gen.appliesToDimension(dimension)) continue;
            if (!gen.appliesToBiome(biomeId)) continue;
            try {
                gen.generate(levelPtr, chunkX, chunkZ, randomPtr);
            } catch (Exception e) {
                System.out.println("[AxoJVM] ERROR: decorate gen failed: " + e.getMessage());
            }
        }
    }
}
