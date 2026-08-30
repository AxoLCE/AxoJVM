package axo.jvm;

import axo.jvm.api.Item;
import axo.jvm.helpers.IdMap;
import axo.jvm.helpers.JsonParser;

import java.nio.file.Path;
import java.util.List;

public class Bridge {
    public static void bootstrap() {
        FileManagment fileManagment = new FileManagment();
        ModLoader modLoader = new ModLoader();
        String gameSide = System.getProperty("axo.game.side", "client").toLowerCase().trim();

        System.out.println("[AxoJVM] Hello from runtime!");
        System.out.println("[AxoJVM] Classpath: " + System.getProperty("java.class.path"));
        System.out.println("[AxoJVM] Runtime gameside: " + System.getProperty("axo.game.side"));
        System.out.println("[AxoJVM] Mods Directory: " + fileManagment.getModsFolder());
        System.out.println("[AxoJVM] Detected mods: " + fileManagment.countValidMods());

        fileManagment.createModsFolder();
        IdMap.load();
        List<Path> detectedMods = fileManagment.detectMods();
        for (Path jarPath : detectedMods){
            String mainClass = JsonParser.readMainClassFromManifest(jarPath);
            String modId = JsonParser.readModIdFromManifest(jarPath);
            String modSide = JsonParser.readSideFromManifest(jarPath).toLowerCase();
            String modName = JsonParser.readNameFromManifest(jarPath);
            if (mainClass == null || mainClass.isEmpty()){
                System.out.println("[AxoJVM] Skipping: "+ jarPath.getFileName());
                continue;
            }
            if (modSide.equals("client") && gameSide.equals("server")){
                System.out.println("Mod: " + modName + " is client side only. Skipping" );
                continue;
            }
            if (modSide.equals("server") && gameSide.equals("client")){
                System.out.println("Mod: " + modName + " is server side only. Skipping");
                continue;
            }
            modLoader.enableMod(jarPath, mainClass, modId);
        }
        modLoader.fireRegistrationEvents();
        registerAllBlocks();
        registerAllItem();
        registerAllBiomes();
        IdMap.save();
        modLoader.enableMods();
    }
    public static void shutdown(){
        ModLoader modLoader = new ModLoader();
        System.out.println("[AxoJVM] Disabling mods");
        modLoader.disableMods();
    }
    public static void registerAllBlocks(){
        BlockRegistry.registerAllTonative();
    }
    public static void registerAllBiomes(){
        BiomeRegistry.registerAllToNative();
    }
    public static native void registerTile(
            int id,
            String name,
            String material,
            float destroyTime,
            float explosionResistance,
            String soundType,
            String iconName,
            boolean isSolidRender,
            int dropItemId,
            String renderShape,
            boolean canWalkThrough,
            String registryName
    );
    public static native void registerItem(
        int id,
        String name,
        String iconName,
        int maxStackSize,
        String registryName
        );
    public static native void registerCrop(
            int id,
            String name,
            String[] stageTextures,
            int seedItemId,
            int dropItemId,
            String registryName
    );
    public static native void registerSeed(
            int id,
            String name,
            String iconName,
            int maxStackSize,
            int plantBlockId,
            String registryName
    );

    public static native int randomInt(long randomPtr, int bound);
    public static native void setRandomSeed(long randomPtr, long seed);
    public static native int getBlock(long levelPtr, int x, int y, int z);
    public static native void setBlock(long levelPtr, int x, int y, int z, int tile);
    public static native void setBlockWithData(long levelPtr, int x, int y, int z, int tile, int data);
    public static native int getBiomeAt(long levelPtr, int x, int z);
    public static native int getSeaLevel(long levelPtr);

    public static native void registerBiomeSpawn(int biomeId, int category, int weight);

    public static native void registerBiome(
            int id,
            String name,
            int grassColor,
            int foliageColor,
            int waterColor,
            int skyColor,
            float temperature,
            float downfall,
            float depth,
            float scale,
            int topBlockId,
            int fillerBlockId
    );

    public static void registerAllItem(){
        ItemRegistry.registerAllToNative();
    }
}