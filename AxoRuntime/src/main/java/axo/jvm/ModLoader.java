package axo.jvm;

import axo.jvm.helpers.ModRegistry;

import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.List;
import java.nio.file.Path;

public class ModLoader {
    private static final List<AxoMod> loadedMods = new ArrayList<>();
    public void enableMod(Path jarPath, String mainClassName, String modId){
        try{
            URL[] urls = {jarPath.toUri().toURL()};
            URLClassLoader loader = new URLClassLoader(urls, Bridge.class.getClassLoader());
            ModRegistry.register(mainClassName, modId);
            Class<?> clazz = loader.loadClass(mainClassName);
            if (AxoMod.class.isAssignableFrom(clazz)){
                AxoMod modInstance = (AxoMod) clazz.getDeclaredConstructor().newInstance();
                modInstance.onEnable();
                loadedMods.add(modInstance);
            } else {
                System.out.println("[AxoJVM] ERROR: Class" + mainClassName + " is broken");
            }
        }catch (Exception e){
            System.out.println("[AxoJVM] ERROR: Failed to load mod: " + e.getMessage());
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
}
