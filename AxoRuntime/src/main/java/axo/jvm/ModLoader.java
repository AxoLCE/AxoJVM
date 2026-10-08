package axo.jvm;

import axo.jvm.event.RegisterBiomeEvent;
import axo.jvm.event.RegisterBlockEvent;
import axo.jvm.event.RegisterItemEvent;
import axo.jvm.event.RegisterWorldGenEvent;
import axo.jvm.helpers.ModRegistry;

import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.time.LocalTime;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.nio.file.Path;
import java.util.Map;

public class ModLoader {
    private static final List<AxoMod> loadedMods = new ArrayList<>();
    private static final Map<AxoMod, String> modIds = new HashMap<>();
    private static final Map<String, URLClassLoader> modClassLoaders = new HashMap<>();
    private static final List<ModInfo> modInfos = new ArrayList<>();

    public static class ModInfo{
        public final String id;
        public final String name;
        public final String version;
        public final String author;
        public final String description;
        public final String status;
        public final String reason;

        public ModInfo(
                String id,
                String name,
                String version,
                String author,
                String description,
                String status,
                String reason
        ){
            this.id = id == null ? "" : id;
            this.name = name == null ? this.id : name;
            this.version = version == null ? "" : version;
            this.author = author == null ? "" : author;
            this.description = description == null ? "" : description;
            this.status = status == null ? "INACTIVE" : status;
            this.reason = reason == null ? "" : reason;
        }

        public String serialize(){
            return id + "\u001f"
                    + name + "\u001f"
                    + version + "\u001f"
                    + author + "\u001f"
                    + description + "\u001f"
                    + status + "\u001f"
                    + reason;
        }
    }

    public static void registerInactive(
            String modId,
            String name,
            String version,
            String author,
            String description,
            String reason
    ){
        modInfos.add(new ModInfo(
                modId,
                name,
                version,
                author,
                description,
                "INACTIVE",
                reason
        ));
    }

    public void enableMod(
            Path jarPath,
            String mainClassName,
            String modId,
            String modName,
            String version,
            String author,
            String description
    ){
        try{
            URL[] urls = {jarPath.toUri().toURL()};
            URLClassLoader loader = new URLClassLoader(urls, Bridge.class.getClassLoader());

            ModRegistry.register(mainClassName, modId);

            Class<?> clazz = loader.loadClass(mainClassName);
            modClassLoaders.put(modId, loader);

            if (AxoMod.class.isAssignableFrom(clazz)){
                AxoMod modInstance = (AxoMod) clazz.getDeclaredConstructor().newInstance();
                modIds.put(modInstance, modId);
                loadedMods.add(modInstance);
                modInfos.add(new ModInfo(
                        modId,
                        modName,
                        version,
                        author,
                        description,
                        "ACTIVE",
                        ""
                ));
            } else {
                registerInactive(
                        modId,
                        modName,
                        version,
                        author,
                        description,
                        "Entrypoint does not implement Axo"
                );
                System.out.println("[AxoJVM] ERROR: Class" + mainClassName + " is broken");
            }
        }catch (Exception e){
            registerInactive(
                    modId,
                    modName,
                    version,
                    author,
                    description,
                    e.getMessage()
            );
            System.out.println("[AxoJVM] ERROR: Failed to load mod: " + e.getMessage());
        }
    }

    public static String[] getModList(){
        String[] result = new String[modInfos.size()];
        for (int i = 0; i<modInfos.size(); i++){
            result[i] = modInfos.get(i).serialize();
        }
        return result;
    }

    public void fireRegistrationEvents(){
        for (AxoMod mod: loadedMods){
            String modId = modIds.get(mod);
            RegisterBlockEvent event = new RegisterBlockEvent(modId);
            RegisterItemEvent event1 = new RegisterItemEvent(modId);
            RegisterBiomeEvent event2 = new RegisterBiomeEvent(modId);
            RegisterWorldGenEvent event3 = new RegisterWorldGenEvent(modId);
            mod.onRegisterBlock(event);
            mod.onRegisterItem(event1);
            mod.onRegisterBiome(event2);
            mod.onRegisterWorldGen(event3);
        }
    }

    public void enableMods(){
        for (AxoMod mod : loadedMods){
            try {
                mod.onEnable();
            }catch (Exception e){

            }
        }
    }
    public void disableMods(){
        System.out.println("[AxoJVM] Shutting down mods...");
        for (AxoMod mod : loadedMods){
            try {
                mod.onDisable();
            }catch (Exception e){
                System.out.println("[AxoJVM] ERROR: Shutting down failed with: " + e.getMessage());
            }
        }
    }

    public static URLClassLoader getModClassLoader(String modId){
        return modClassLoaders.get(modId);
    }
}
