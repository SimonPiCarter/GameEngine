#pragma once

#include "message/MessageHandler.h"

#include "message/BlocMessage.h"

class BlocEngine;

class BlocMessageHandler : public MessageHandler<BlocMessage>
{
public:
	BlocMessageHandler(BlocEngine *engine_p);

	void visitPopBloc(PopBlocMessage const &msg_p);

	void visitSpawnBloc(SpawnBlocMessage const &msg_p);

	void visitFreezeBloc(FreezeBlocMessage const &msg_p);

private:
	BlocEngine *_engine;

protected:
	/// @brief method to be extended to handle message
	virtual void handleMessage(BlocMessage *msg_p) override { msg_p->visit(*this); }
};
