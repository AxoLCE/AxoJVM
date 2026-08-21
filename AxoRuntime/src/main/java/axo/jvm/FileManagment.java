package axo.jvm;

import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.nio.file.FileSystem;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Stream;

public class FileManagment {

    private Path resolveModsPath() throws URISyntaxException {
        Path jarPath = Path.of(Bridge.class.getProtectionDomain().getCodeSource().getLocation().toURI());
        Path axoFolder = jarPath.getParent();
        Path gameFolder = axoFolder.getParent();
        return gameFolder.resolve("mods");
    }

    public void createModsFolder() {
        try {
            Path modsFolder = resolveModsPath();
            Files.createDirectories(modsFolder);
        } catch (URISyntaxException | IOException e) {
            System.err.println("[AxoJVM] ERROR: Mods folder creation failed: " + e.getMessage());
        }
    }

    public Path getModsFolder() {
        try {
            return resolveModsPath();
        } catch (URISyntaxException e) {
            System.err.println("[AxoJVM] ERROR: Could not resolve mods path: " + e.getMessage());
            return null;
        }
    }

    public List<Path> detectMods() {
        Path modsFolder = getModsFolder();
        if (modsFolder == null || !Files.exists(modsFolder)){
            return Collections.emptyList();
        }

        List<Path> detectedMods = new ArrayList<>();
        try (Stream<Path> stream = Files.list(getModsFolder())) {
            stream.filter(path -> path.toString().endsWith(".jar")).forEach(jarPath -> {
                if (hasModJson(jarPath)){
                    detectedMods.add(jarPath);
                }
            });
        } catch (IOException e) {
            System.err.println("[AxoJVM] ERROR: Mods folder loading failed:" + e.getMessage());
        }
        return detectedMods;
    }
    private static boolean hasModJson(Path jarPath) {
        try (java.util.jar.JarFile jarFile = new java.util.jar.JarFile(jarPath.toFile())) {
            java.util.jar.JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            return entry != null;
        } catch (IOException e) {
            System.err.println("[AxoJVM] ERROR: Failed to open JAR: " + jarPath.getFileName() + " with error: " + e.getMessage());
            return false;
        }
    }

    public int countValidMods() {
        Path modsFolder = getModsFolder();
        if (modsFolder == null || !Files.exists(modsFolder)){
            return 0;
        }
        return detectMods().size();
    }
}
