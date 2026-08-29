package axo.jvm;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

public class LangRegistry {
    private static final Map<String, String> TRANSLATIONS = new HashMap<>();

    public static void loadLang(String modId, String langCode, String resourcePath){
        try{
            InputStream is = ModLoader.getModClassLoader(modId).getResourceAsStream(resourcePath);
            if (is == null){
                System.out.println("[AxoJVM] ERROR: Lang file not found: " + resourcePath);
                return;
            }
            BufferedReader reader = new BufferedReader(new InputStreamReader(is, StandardCharsets.UTF_8));
            String line;
            while ((line = reader.readLine()) != null){
                line = line.trim();
                if (line.isEmpty() || line.startsWith("#")){
                    continue;
                }
                int eq = line.indexOf('=');
                if (eq > 0){
                    String key = line.substring(0, eq).trim();
                    String value = line.substring(eq + 1).trim();
                    TRANSLATIONS.put(key, value);
                }
            }
            reader.close();
            System.out.println("[AxoJVM] Loaded lang: " + resourcePath);
        } catch (Exception e){
            System.out.println("[AxoJVM] ERROR: Failed to load lang: " + e.getMessage());
        }
    }

    public static String get(String key){
        return TRANSLATIONS.getOrDefault(key, key);
    }
}
