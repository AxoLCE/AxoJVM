package axo.jvm.event;

import axo.jvm.BiomeRegistry;
import axo.jvm.api.Biome;

public class RegisterBiomeEvent {
    private final String modId;
    public RegisterBiomeEvent(String modId){
        this.modId = modId;
    }
    public void register(Biome biome){
        BiomeRegistry.register(biome, modId);
    }
}
