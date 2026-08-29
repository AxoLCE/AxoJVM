#include "AxoBridge.h"
#include "AxoTileItem.h"

std::wstring AxoTileItem::getHoverName(std::shared_ptr<ItemInstance> itemInstance) {
    std::wstring wKey = L"block." + m_registryName + L".name";
    std::string key(wKey.begin(), wKey.end());
    
    std::wstring translated = AxoBridge_GetLang(key);
    if (translated != wKey) {
        return translated;
    }
    return m_displayName;
}
