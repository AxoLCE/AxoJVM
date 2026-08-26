package axo.jvm.helpers;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;
import java.util.concurrent.atomic.AtomicInteger;

public class IdMap {
    private static final Path MAP_FILE = Paths.get("axojvm","axojvm-id-map.json");
    private static final int MIN_TILE_ID = 217;
    private static final int MAX_TILE_ID = 254;
    private static final Map<String, Integer> TILE_IDS = new HashMap<>();
    private static final AtomicInteger NEXT_TILE_ID = new AtomicInteger(MIN_TILE_ID);
    private static final Map<String, Integer> ITEM_IDS = new HashMap<>();
    private static final AtomicInteger NEXT_ITEM_ID = new AtomicInteger(1000);

    public static void load(){
        if(!Files.exists(MAP_FILE)){
            System.out.println("[AxoJVM] No existing ID map, starting new");
            return;
        }
        try(BufferedReader reader = Files.newBufferedReader(MAP_FILE)){
            String jsonText = reader.lines().collect(Collectors.joining("\n"));
            String tilesJson = extractJsonObject(jsonText, "tiles");
            String itemsJson = extractJsonObject(jsonText, "items");
            if (tilesJson != null){
                parseAndPopulateMap(tilesJson, TILE_IDS, NEXT_TILE_ID);
            }
            if (itemsJson != null){
                parseAndPopulateMap(itemsJson, ITEM_IDS, NEXT_ITEM_ID);
            }
        }catch (Exception e){
            System.out.println("[AxoJVM] Failed to load ID map: " + e.getMessage());
        }
    }

    public static void save(){
        try {
            Files.createDirectories(MAP_FILE.getParent());
            try(BufferedWriter writer = Files.newBufferedWriter(MAP_FILE)){
                writer.write("{\n");
                writer.write("  \"tiles\": {\n");
                serializeMap(writer, TILE_IDS);
                writer.write("  },\n");

                writer.write("  \"items\": {\n");
                serializeMap(writer, ITEM_IDS);
                writer.write("  }\n");
                writer.write("}");
            }
        }catch (Exception e){
            System.out.println("[AxoJVM] Failed to save ID map: " + e.getMessage());
        }
    }

    private static String extractJsonObject(String json, String key){
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
    private static void parseAndPopulateMap(String sectionContent, Map<String, Integer> targetMap, AtomicInteger nextIdTracker){
        String[] pairs = sectionContent.split(",");
        for (String pair : pairs){
            pair = pair.trim();
            if (pair.isEmpty()) continue;
            String[] kv = pair.split(":", 2);
            if (kv.length != 2) continue;
            String rawKey = kv[0].trim();
            int firstQuote = rawKey.indexOf("\"");
            int lastQuote = rawKey.lastIndexOf("\"");
            if (firstQuote == -1 || lastQuote == -1 || firstQuote == lastQuote) continue;
            String key = rawKey.substring(firstQuote +1,lastQuote);
            try {
                int id = Integer.parseInt(kv[1].trim());
                targetMap.put(key, id);
                if (id >= nextIdTracker.get()){
                    nextIdTracker.set(id +1);
                }
            }catch (NumberFormatException e){

            }
        }
    }

    private static void serializeMap(BufferedWriter writer, Map<String, Integer> map) throws IOException{
        List<Map.Entry<String, Integer>> entries = new ArrayList<>(map.entrySet());
        for (int i = 0; i <entries.size(); i++){
            Map.Entry<String, Integer> entry = entries.get(i);
            writer.write("    \"" + entry.getKey() + "\": " + entry.getValue());
            if (i < entries.size() -1 ){
                writer.write(",");
            }
            writer.newLine();
        }
    }

    public static int getOrAllocateTileId(String modId, String blockName){
        String key = modId + ":" + blockName;
        Integer existing = TILE_IDS.get(key);
        if (existing != null){
            return existing;
        }
        int newId = NEXT_TILE_ID.getAndIncrement();
        if (newId > MAX_TILE_ID){
            throw new IllegalStateException(
                    "[AxoJVM] Out of tile ID slots!"
            );
        }
        TILE_IDS.put(key, newId);
        return newId;
    }
    public static int getOrAllocateItemId(String modId, String itemName){
        String key = modId + ":" + itemName;
        Integer existing = ITEM_IDS.get(key);
        if (existing != null){
            return existing;
        }
        int newId = NEXT_ITEM_ID.getAndIncrement();
        ITEM_IDS.put(key, newId);
        return newId;
    }
    public static Integer getTileId(String modId, String blockName){
        return TILE_IDS.get(modId + ":" + blockName);
    }
    public static Integer getItemId(String modId, String itemName){
        return ITEM_IDS.get(modId + ":" + itemName);
    }
}
