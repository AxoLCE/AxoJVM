package axo.jvm;

import axo.jvm.event.RegisterBlockEvent;
import axo.jvm.event.RegisterItemEvent;

public interface AxoMod {
    void onEnable();
    void onDisable();
    void onRegisterBlock(RegisterBlockEvent event);
    void onRegisterItem(RegisterItemEvent event);
}
