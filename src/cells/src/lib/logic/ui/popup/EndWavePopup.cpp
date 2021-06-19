#include "EndWavePopup.h"

#include "logic/LogicEngine.h"
#include "game/CellsEngine.h"

EndWavePopup::EndWavePopup(LogicEngine & engine_p, EndWavePopup * next_p) : PopUpScreen(), _engine(engine_p), _next(next_p)
{}

EndWavePopup::~EndWavePopup() {}

void EndWavePopup::initEndWavePopup()
{
    init(getButtons(), _engine.getCellsEngine()->getGraphic());
}

void EndWavePopup::cleanEndWavePopup()
{
    clean();
}

void start_popup(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p)
{
    EndWavePopup * popup_l = dynamic_cast<EndWavePopup *>(toolkit_p);
    if(popup_l)
    {
        popup_l->initEndWavePopup();
    }
}

void end_popup(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p)
{
    EndWavePopup * popup_l = dynamic_cast<EndWavePopup *>(toolkit_p);
    if(popup_l)
    {
        popup_l->cleanEndWavePopup();
    }
}

void NextListener::run()
{
    _engine.closePopup();
    if(_next)
    {
         _engine.openPopup(_next);
    }
}
