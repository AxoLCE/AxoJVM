#pragma once
#include "../TileItem.h"
#include <string>
#include <memory>

class AxoTileItem : public TileItem {
private:
    std::wstring m_displayName;
    std::wstring m_registryName;

public:
    AxoTileItem(int id, const std::wstring& displayName, const std::wstring& registryName) 
        : TileItem(id), m_displayName(displayName), m_registryName(registryName) {}
    virtual std::wstring getHoverName(std::shared_ptr<ItemInstance> itemInstance) override;
};
