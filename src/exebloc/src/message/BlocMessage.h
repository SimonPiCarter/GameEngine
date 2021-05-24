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
	PopBlocMessage(std::set<GraphicEntity*> const &set_p, std::array<size_t, 3> const &pos_p,
		bool popX_p, bool popY_p)  : _set(set_p), _pos(pos_p), _popX(popX_p), _popY(popY_p) {}

	std::set<GraphicEntity*> const & getSet() const { return _set; }
	std::array<size_t, 3> const &getPos() const { return _pos; }
	bool const &getPopX() const { return _popX; }
	bool const &getPopY() const { return _popY; }

	/// @brief visitor method
	virtual void visit(BlocMessageHandler &handler_p);
private:
	std::set<GraphicEntity*> const _set;
	std::array<size_t, 3> const _pos;
	bool const _popX;
	bool const _popY;
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
