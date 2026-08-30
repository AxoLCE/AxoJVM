package axo.jvm;

import axo.jvm.event.RegisterBiomeEvent;
import axo.jvm.event.RegisterBlockEvent;
import axo.jvm.event.RegisterItemEvent;
import axo.jvm.event.RegisterWorldGenEvent;

public interface AxoMod {
    void onEnable();
    void onDisable();
    void onRegisterBlock(RegisterBlockEvent event);
    void onRegisterItem(RegisterItemEvent event);
    void onRegisterBiome(RegisterBiomeEvent event);
    void onRegisterWorldGen(RegisterWorldGenEvent event);
}
