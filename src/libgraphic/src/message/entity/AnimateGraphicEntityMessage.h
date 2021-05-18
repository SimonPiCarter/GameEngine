#pragma once

#include "message/GraphicMessage.h"

#include <string>
#include <array>


class GraphicEntity;

class AnimateGraphicEntityMessage : public GraphicMessage
{
public:
	AnimateGraphicEntityMessage(GraphicEntity * entity_p, std::string const &animation_p, bool loop_p, bool enable_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	GraphicEntity *getEntity() const { return _entity; }
	std::string const & getAnimation() const { return _animation; }
	bool const & isLoop() const { return _loop; }
	bool const & isEnable() const { return _enable; }

private:
	GraphicEntity * const _entity;
	const std::string _animation;
	const bool _loop;
	const bool _enable;
};
