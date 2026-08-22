package axo.jvm;

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
    }
}