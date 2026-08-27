package axo.jvm;

import axo.jvm.event.RegisterBlockEvent;
import axo.jvm.event.RegisterItemEvent;
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
    public void enableMod(Path jarPath, String mainClassName, String modId){
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
            } else {
                System.out.println("[AxoJVM] ERROR: Class" + mainClassName + " is broken");
            }
        }catch (Exception e){
            System.out.println("[AxoJVM] ERROR: Failed to load mod: " + e.getMessage());
        }
    }

    public void fireRegistrationEvents(){
        for (AxoMod mod: loadedMods){
            String modId = modIds.get(mod);
            RegisterBlockEvent event = new RegisterBlockEvent(modId);
            RegisterItemEvent event1 = new RegisterItemEvent(modId);
            mod.onRegisterBlock(event);
            mod.onRegisterItem(event1);
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
