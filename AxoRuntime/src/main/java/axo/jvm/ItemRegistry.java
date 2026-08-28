package axo.jvm;

import axo.jvm.api.Item;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ItemRegistry {
    private static final Map<Integer, Item> BY_ID = new HashMap<>();
    private static final Map<String, Item> BY_NAME = new HashMap<>();
    private static final List<Item> ALL = new ArrayList<>();

    public static void register(Item item, String modId){
        if (item.getId() != -1){
            throw new IllegalStateException("Already Registred");
        }
        item.assignRegistry(modId);
        BY_ID.put(item.getId(), item);
        BY_NAME.put(item.getName(), item);
        ALL.add(item);
    }

    public static int getItemIdByName(String name){
        Item item = BY_NAME.get(name);
        return item != null ? item.getId() : 1;
    }

    public static void registerAllToNative(){
        for (Item item : ALL) {
            if (item instanceof Item) {
                if (item.getProperties().plantBlockId != -1) {
                    Bridge.registerSeed(
                            item.getId(),
                            item.getName(),
                            item.getIconName(),
                            item.getMaxStackSize(),
                            item.getProperties().plantBlockId
                    );
                } else {
                    Bridge.registerItem(
                            item.getId(),
                            item.getName(),
                            item.getIconName(),
                            item.getMaxStackSize()
                    );
                }
            }
        }
    }

}
