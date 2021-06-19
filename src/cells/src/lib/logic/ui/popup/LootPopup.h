#pragma once

#include "EndWavePopup.h"
#include "logic/ui/layout/LootLayout.h"

class LootPopup : public EndWavePopup
{
public:
    LootPopup(LootLayout const &layout_p, LogicEngine & engine_p, EndWavePopup * next_p);

    virtual void setContent();

    virtual std::vector<ButtonData> getButtons();
private:
    LootLayout const _layout;
};
