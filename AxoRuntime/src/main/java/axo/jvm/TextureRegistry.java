package axo.jvm;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

public class TextureRegistry {
    private static final int START_SLOT_BLOCK = 400;
    private static final int START_SLOT_ITEM = 250;
    private static int nextSlotBlock = START_SLOT_BLOCK;
    private static int nextSlotItem = START_SLOT_ITEM;

    public static class CustomTexture {
        public final String name;
        public final byte[] pngBytes;
        public final int slot;
        public final int row;
        public final int col;
        public final int atlasType;
        public final float u0;
        public final float v0;
        public final float u1;
        public final float v1;

        public CustomTexture(String name, byte[] pngBytes, int atlasType) {
            this.name = name;
            this.pngBytes = pngBytes;
            this.atlasType = atlasType;

            if (atlasType == 0) {
                this.slot = nextSlotBlock++;
            } else {
                this.slot = nextSlotItem++;
            }

            this.row = this.slot / 16;
            this.col = this.slot % 16;

            float atlasHeight = (atlasType == 0) ? 512.0f : 256.0f;
            float padding = 0.001f;

            this.u0 = ((col * 16.0f) / 256.0f) + padding;
            this.v0 = ((row * 16.0f) / atlasHeight) + padding;
            this.u1 = (((col + 1) * 16.0f) / 256.0f) - padding;
            this.v1 = (((row + 1) * 16.0f) / atlasHeight) - padding;
        }
    }

    private static final Map<String, CustomTexture> TEXTURES = new HashMap<>();

    public static void register(String modId, String name, String resourcePath, int atlasType) {
        String fullName = "axo:" + modId + ":" + name;
        if (TEXTURES.containsKey(fullName)) return;

        try {
            InputStream is = ModLoader.getModClassLoader(modId).getResourceAsStream(resourcePath);
            if (is == null) {
                System.out.println("[AxoJVM] ERROR: Texture: " + resourcePath + " not found");
                return;
            }
            byte[] bytes = is.readAllBytes();
            is.close();

            TEXTURES.put(fullName, new CustomTexture(fullName, bytes, atlasType));
            System.out.println("[AxoJVM] Registred texture: " + fullName);
        } catch (IOException e) {
            System.out.println("[AxoJVM] Failed registering PNG" + e.getMessage());
        }
    }

    public static Map<String, CustomTexture> getAll() {
        return TEXTURES;
    }
}