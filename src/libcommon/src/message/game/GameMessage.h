#pragma once

#include <string>
#include <SDL.h>

class GameMessageHandler;

class GameMessage
{
public:
	GameMessage(std::string const &msg_p);
	virtual ~GameMessage();

	std::string const & getMsg() const;

	/// @brief visitor method
	virtual void visit(GameMessageHandler &handler_p) = 0;

private:
	std::string const _msg;
};

class NoOpGameMessage : public GameMessage
{
public:
	NoOpGameMessage(std::string const &msg_p);

	virtual void visit(GameMessageHandler &handler_p);
};

class SDLEventGameMessage : public GameMessage
{
public:
	SDLEventGameMessage(SDL_Event const &event_p);

	virtual void visit(GameMessageHandler &handler_p);

	SDL_Event const &getEvent() const;
protected:
	SDL_Event const _event;
};
