package axo.jvm.helpers;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Path;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.stream.Collectors;

public class JsonParser {

    public static String readMainClassFromManifest(Path jarPath){
        try (JarFile jarFile = new JarFile(jarPath.toFile())){
            JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            if (entry == null) return null;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)))){
                String jsonText = reader.lines().collect(Collectors.joining("\n"));
                return extractValue(jsonText, "entrypoint");
            }
        }catch (IOException e){
            System.err.println("[AxoJVM] ERROR: Failed to parse manifest: " + jarPath.getFileName());
            return null;
        }
    }
    public static String readModIdFromManifest(Path jarPath){
        try (JarFile jarFile = new JarFile(jarPath.toFile())){
            JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            if (entry == null) return null;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)))){
                String jsonText = reader.lines().collect(Collectors.joining("\n"));
                return extractValue(jsonText, "modId");
            }
        }catch (IOException e){
            System.err.println("[AxoJVM] ERROR: Failed to parse manifest: " + jarPath.getFileName());
            return null;
        }
    }
    public static String readVersionFromManifest(Path jarPath){
        try (JarFile jarFile = new JarFile(jarPath.toFile())){
            JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            if (entry == null) return null;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)))){
                String jsonText = reader.lines().collect(Collectors.joining("\n"));
                return extractValue(jsonText, "version");
            }
        }catch (IOException e){
            System.err.println("[AxoJVM] ERROR: Failed to parse manifest: " + jarPath.getFileName());
            return null;
        }
    }
    public static String readNameFromManifest(Path jarPath){
        try (JarFile jarFile = new JarFile(jarPath.toFile())){
            JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            if (entry == null) return null;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)))){
                String jsonText = reader.lines().collect(Collectors.joining("\n"));
                return extractValue(jsonText, "name");
            }
        }catch (IOException e){
            System.err.println("[AxoJVM] ERROR: Failed to parse manifest: " + jarPath.getFileName());
            return null;
        }
    }
    public static String readAuthorFromManifest(Path jarPath){
        try (JarFile jarFile = new JarFile(jarPath.toFile())){
            JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            if (entry == null) return null;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)))){
                String jsonText = reader.lines().collect(Collectors.joining("\n"));
                return extractValue(jsonText, "author");
            }
        }catch (IOException e){
            System.err.println("[AxoJVM] ERROR: Failed to parse manifest: " + jarPath.getFileName());
            return null;
        }
    }
    public static String readDescriptionFromManifest(Path jarPath){
        try (JarFile jarFile = new JarFile(jarPath.toFile())){
            JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            if (entry == null) return null;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)))){
                String jsonText = reader.lines().collect(Collectors.joining("\n"));
                return extractValue(jsonText, "description");
            }
        }catch (IOException e){
            System.err.println("[AxoJVM] ERROR: Failed to parse manifest: " + jarPath.getFileName());
            return null;
        }
    }
    public static String readSideFromManifest(Path jarPath){
        try (JarFile jarFile = new JarFile(jarPath.toFile())){
            JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            if (entry == null) return null;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)))){
                String jsonText = reader.lines().collect(Collectors.joining("\n"));
                return extractValue(jsonText, "side");
            }
        }catch (IOException e){
            System.err.println("[AxoJVM] ERROR: Failed to parse manifest: " + jarPath.getFileName());
            return null;
        }
    }
    public static String readEntrypointFromManifest(Path jarPath){
        try (JarFile jarFile = new JarFile(jarPath.toFile())){
            JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            if (entry == null) return null;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)))){
                String jsonText = reader.lines().collect(Collectors.joining("\n"));
                return extractValue(jsonText, "entrypoint");
            }
        }catch (IOException e){
            System.err.println("[AxoJVM] ERROR: Failed to parse manifest: " + jarPath.getFileName());
            return null;
        }
    }
    public static String readModIconFromManifest(Path jarPath){
        try (JarFile jarFile = new JarFile(jarPath.toFile())){
            JarEntry entry = jarFile.getJarEntry("axo.mod.json");
            if (entry == null) return null;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)))){
                String jsonText = reader.lines().collect(Collectors.joining("\n"));
                return extractValue(jsonText, "modIcon");
            }
        }catch (IOException e){
            System.err.println("[AxoJVM] ERROR: Failed to parse manifest: " + jarPath.getFileName());
            return null;
        }
    }

    private static String extractValue(String json, String key){
        String searchStr =  "\"" + key + "\"";
        int keyIndex = json.indexOf(searchStr);
        if (keyIndex == -1) return null;

        int colonIndex = json.indexOf(":", keyIndex + searchStr.length());
        if (colonIndex == -1) return null;

        int openQuote = json.indexOf("\"", colonIndex);
        if (openQuote == -1) return null;

        int closeQuote = json.indexOf("\"", openQuote + 1);
        if (closeQuote == -1) return null;

        return json.substring(openQuote +1, closeQuote).trim();
    }
}
