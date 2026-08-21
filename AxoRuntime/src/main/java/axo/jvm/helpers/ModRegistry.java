package axo.jvm.helpers;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class ModRegistry {
    private static final Map<String, String> LOADED_MODS = new ConcurrentHashMap<>();

    public static void register(String mainClassName, String modId) {
        LOADED_MODS.put(mainClassName, modId);
    }

    public static String getModId(String className) {
        return LOADED_MODS.getOrDefault(className, "System");
    }
}
