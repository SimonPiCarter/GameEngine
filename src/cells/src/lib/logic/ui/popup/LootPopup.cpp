#include "LootPopup.h"

#include "logic/utils/Translator.h"

#include "ColibriGui/Layouts/ColibriLayoutLine.h"

LootPopup::LootPopup(LootLayout const &layout_p, LogicEngine & engine_p, EndWavePopup * next_p) : EndWavePopup(engine_p, next_p), _layout(layout_p) {}

void LootPopup::setContent()
{
    Colibri::LayoutLine *layoutV = new Colibri::LayoutLine( _manager );
    layoutV->m_vertical = true;

    Colibri::Label *title_l = _manager->createWidget<Colibri::Label>( _window );
    title_l->setText(_layout.title);
    title_l->sizeToFit(_manager->getCanvasSize().x*0.75);
    layoutV->addCell(title_l);

    Colibri::Label *text_l = _manager->createWidget<Colibri::Label>( _window );
    text_l->setText(_layout.text);
    text_l->sizeToFit(_manager->getCanvasSize().x*0.75);
    layoutV->addCell(text_l);

    layoutV->setAdjustableWindow(_window);
    layoutV->m_hardMaxSize = _manager->getCanvasSize();
    layoutV->layout();
}

std::vector<ButtonData> LootPopup::getButtons()
{
    std::vector<ButtonData> buttons_l;
    buttons_l.push_back({translate("OK"), new NextListener(_engine, _next), nullptr, ""});
    return buttons_l;
}
