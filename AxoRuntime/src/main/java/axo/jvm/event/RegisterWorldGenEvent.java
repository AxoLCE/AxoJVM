package axo.jvm.event;

import axo.jvm.WorldGenRegistry;
import axo.jvm.api.WorldGen;

public class RegisterWorldGenEvent {
    private final String modId;
    public RegisterWorldGenEvent(String modId){
        this.modId = modId;
    }
    public void register(WorldGen gen){
        WorldGenRegistry.register(gen, modId);
    }
}
