package axo.jvm.event;

import axo.jvm.BlockRegistry;
import axo.jvm.api.Block;

public class RegisterBlockEvent {
    private final String modId;
    public RegisterBlockEvent(String modId){
        this.modId = modId;
    }
    public void register(Block block){
        BlockRegistry.register(block, modId);
    }
    public String getModId() {
        return modId;
    }
}
