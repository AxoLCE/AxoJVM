#pragma once
#include "../TileItem.h"

class AxoTileItem : public TileItem {
private:
    wstring m_displayName;
public:
    AxoTileItem(int id, const wstring& displayName) : TileItem(id) {
        m_displayName = displayName;
    }
    
    virtual wstring getHoverName(shared_ptr<ItemInstance> itemInstance) override {
        return m_displayName;
    }
};