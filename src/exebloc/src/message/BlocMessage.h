#pragma once

#include <array>
#include <set>

class BlocMessageHandler;
class GraphicEntity;
class Bloc;
class BlocModel;

class BlocMessage
{
public:
	virtual ~BlocMessage() {}

	/// @brief visitor method
	virtual void visit(BlocMessageHandler &handler_p) = 0;

};

class PopBlocMessage : public BlocMessage
{
public:
	PopBlocMessage(std::set<GraphicEntity*> const &set_p)  : _set(set_p) {}

	std::set<GraphicEntity*> const & getSet() const { return _set; }

	/// @brief visitor method
	virtual void visit(BlocMessageHandler &handler_p);
private:
	std::set<GraphicEntity*> const _set;
};

class SpawnBlocMessage : public BlocMessage
{
public:
	SpawnBlocMessage(BlocModel const &model_p, std::array<unsigned long, 3> const &position_p);

	/// @brief _model getter
	BlocModel const &getModel() const;

	/// @brief _position getter
	std::array<unsigned long, 3> const &getPosition() const;

	/// @brief visitor method
	virtual void visit(BlocMessageHandler &handler_p);

private:
	BlocModel const & _model;
	std::array<unsigned long, 3> const _position;
};

class FreezeBlocMessage : public BlocMessage
{
public:
	FreezeBlocMessage(Bloc * bloc_p);

	/// @brief _bloc getter
	Bloc * getBloc() const;

	/// @brief visitor method
	virtual void visit(BlocMessageHandler &handler_p);

private:
	Bloc * const _bloc;
};
