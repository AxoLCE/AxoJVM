package axo.jvm;

import axo.jvm.event.RegisterBlockEvent;

public interface AxoMod {
    void onEnable();
    void onDisable();
    void onRegisterBlock(RegisterBlockEvent event);
}
