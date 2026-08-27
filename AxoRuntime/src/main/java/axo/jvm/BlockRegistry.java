package axo.jvm;

import axo.jvm.api.Block;

import java.util.*;

public class BlockRegistry {
    private static final Map<Integer, Block> BY_ID = new HashMap<>();
    private static final Map<String, Block> BY_NAME = new HashMap<>();
    private static final List<Block> ALL = new ArrayList<>();

    public static void register(Block block, String modId){
        if (block.getId() != -1){
            throw new IllegalStateException("Block already registred: " + block.getRegistryName());
        }
        block.assignRegistry(modId);
        if (BY_ID.containsKey(block.getId())){
            throw new IllegalStateException("Block id " + block.getId() + " already registered by " + BY_ID.get(block.getId()).getRegistryName());
        }
        BY_ID.put(block.getId(), block);
        BY_NAME.put(block.getRegistryName(), block);
        ALL.add(block);
        System.out.println("[AxoJVM] Registered block: " + block.getRegistryName() + " id " + block.getId() + " (icon: " + block.getIconName() + ")");
    }

    public static void registerAllTonative(){
        System.out.println("[AxoJVM] Registering " + BY_ID.size() + " blocks to native...");
        for (Block block : ALL){
            Bridge.registerTile(
                    block.getId(),
                    block.getName(),
                    block.getMaterial(),
                    block.getDestroyTime(),
                    block.getExplosionResistance(),
                    block.getSoundType(),
                    block.getIconName(),
                    block.isSolidRender(),
                    block.getDropItemId()
            );
        }
        System.out.println("[AxoJVM] Native registration complete");
    }
    public static Block get(int id){
        return BY_ID.get(id);
    }
    public static Block getByName(String registryName){
        return BY_NAME.get(registryName);
    }
    public static Collection<Block> all(){
        return ALL;
    }
}
