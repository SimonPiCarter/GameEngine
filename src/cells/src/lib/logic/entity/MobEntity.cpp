#include "MobEntity.h"

#include "logic/layout/MapLayout.h"
#include "logic/layout/MobModelLayout.h"

MobEntity::MobEntity(std::array<double, 2> const &position_p, MobModel const * model_p, MapLayout const &map_p, double spawntime_p)
	: LogicEntity<MobEntity>(position_p, model_p->size)
	, _model(model_p)
	, _hitpoint(_model->hitpoint)
	, _speed(_model->speed)
	, _damageMutliplier(1.)
	, _armorType(_model->armor_type)
	, _checkpoints(map_p.getCheckPoints())
	, _spawntime(spawntime_p)
	, _disabled(false)
{}
