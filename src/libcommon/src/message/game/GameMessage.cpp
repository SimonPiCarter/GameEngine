#include "GameMessage.h"

#include "GameMessageHandler.h"

GameMessage::GameMessage(std::string const &msg_p)
	: _msg(msg_p)
{}

GameMessage::~GameMessage()
{}

std::string const & GameMessage::getMsg() const
{
	return _msg;
}

NoOpGameMessage::NoOpGameMessage(std::string const &msg_p)
	: GameMessage(msg_p)
{}

void NoOpGameMessage::visit(GameMessageHandler &handler_p)
{
	handler_p.visitNoOp(*this);
}

SDLEventGameMessage::SDLEventGameMessage(SDL_Event const &event_p)
	: GameMessage("")
	, _event(event_p)
{}

void SDLEventGameMessage::visit(GameMessageHandler &handler_p)
{
	handler_p.visitSDLEvent(*this);
}

SDL_Event const &SDLEventGameMessage::getEvent() const
{
	return _event;
}
