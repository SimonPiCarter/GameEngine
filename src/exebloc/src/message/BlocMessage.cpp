#include "BlocMessage.h"

#include "engine/BlocEngine.h"

void PopBlocMessage::visit(BlocMessageHandler &handler_p)
{
	handler_p.visitPopBloc(*this);
}

SpawnBlocMessage::SpawnBlocMessage(BlocModel const &model_p, std::array<unsigned long, 3> const &position_p)
	: _model(model_p), _position(position_p)
{}

/// @brief _model getter
BlocModel const &SpawnBlocMessage::getModel() const
{
	return _model;
}

/// @brief _position getter
std::array<unsigned long, 3> const &SpawnBlocMessage::getPosition() const
{
	return _position;
}

void SpawnBlocMessage::visit(BlocMessageHandler &handler_p)
{
	handler_p.visitSpawnBloc(*this);
}

FreezeBlocMessage::FreezeBlocMessage(Bloc * bloc_p) : _bloc(bloc_p) {}

Bloc * FreezeBlocMessage::getBloc() const
{
	return _bloc;
}

void FreezeBlocMessage::visit(BlocMessageHandler &handler_p)
{
	handler_p.visitFreezeBloc(*this);
}

RotateBlocMessage::RotateBlocMessage(Bloc * bloc_p) : _bloc(bloc_p) {}

Bloc * RotateBlocMessage::getBloc() const
{
	return _bloc;
}

void RotateBlocMessage::visit(BlocMessageHandler &handler_p)
{
	handler_p.visitRotateBloc(*this);
}
