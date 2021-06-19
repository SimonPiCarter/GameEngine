#include "LootPopup.h"

#include "logic/utils/Translator.h"

LootPopup::LootPopup(LootLayout const &layout_p, LogicEngine & engine_p, EndWavePopup * next_p) : EndWavePopup(engine_p, next_p), _layout(layout_p) {}

void LootPopup::setContent()
{

}

std::vector<ButtonData> LootPopup::getButtons()
{
    std::vector<ButtonData> buttons_l;
    buttons_l.push_back({translate("OK"), new NextListener(_engine, _next), nullptr, ""});
    return buttons_l;
}
