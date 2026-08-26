package axo.jvm.event;

import axo.jvm.ItemRegistry;
import axo.jvm.api.Item;

public class RegisterItemEvent {
    private final String modId;
    public RegisterItemEvent(String modId){
        this.modId = modId;
    }
    public void register(Item item){
        ItemRegistry.register(item, modId);
    }
}
