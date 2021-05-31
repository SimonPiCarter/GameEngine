#pragma once

#include "LogicEntityDefs.h"
#include "logic/types/ArmorType.h"

class MapLayout;
class MobModel;

class MobEntity : public LogicEntity<MobEntity>
{
public:
	MobEntity(std::array<double, 2> const &position_p, MobModel const * model_p, MapLayout const &map_p);

	MobModel const * getModel() const {return _model;}
	double const &getHitpoint() const {return _hitpoint;}
	double const &getSpeed() const {return _speed;}
	double const &getDamageMutliplier() const {return _damageMutliplier;}
	ArmorType const &getArmorType() const {return _armorType;}
	std::list<std::array<double, 2> > const &getCheckpoints() const {return _checkpoints;}
	std::list<std::array<double, 2> > &getCheckpoints() {return _checkpoints;}

	void setHitpoint(double hitpoint_p) {_hitpoint = hitpoint_p;}
	void setSpeed(double speed_p) {_speed = speed_p;}
	void setDamageMutliplier(double damageMutliplier_p) {_damageMutliplier = damageMutliplier_p;}

protected:
	MobModel const * const _model;
	double _hitpoint;
	double _speed;
	double _damageMutliplier;
	ArmorType _armorType;
	std::list<std::array<double, 2> > _checkpoints;
};
