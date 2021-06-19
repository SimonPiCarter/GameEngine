#pragma once

#include "gui/PopUpScreen.h"
#include "message/gui/CustomGuiMessage.h"

class LogicEngine;

void start_popup(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);
void end_popup(CustomGuiToolkit * toolkit_p, GraphicEngine * graphic_p);

class EndWavePopup : public PopUpScreen, public CustomGuiToolkit
{
public:
    EndWavePopup(LogicEngine & engine_p, EndWavePopup * next_p);
    virtual ~EndWavePopup();

    virtual void initEndWavePopup();
    virtual void cleanEndWavePopup();

    virtual std::vector<ButtonData> getButtons() = 0;
protected:
    LogicEngine & _engine;
    EndWavePopup * const _next;
};

class NextListener : public Listener
{
public:
    NextListener(LogicEngine & engine_p, EndWavePopup * next_p) : _engine(engine_p), _next(next_p) {}

	virtual void run();
protected:
    LogicEngine & _engine;
    EndWavePopup * const _next;
};