#pragma once

#include <array>
#include <list>

class MobEntity;
class WaveEngine;

struct MobMoveInfo
{
	MobEntity * entity;
	std::array<double,2> pos;
	std::array<double,2> dir;
};

class MobMover
{
public:
	MobMover(WaveEngine &waveEngine_p);

	void moveEntities(std::list<MobEntity *> mobs_p, double elapsedTime_p);
protected:
	WaveEngine &_waveEngine;

	MobMoveInfo computeMoveInfo(MobEntity * entity_l, double elapsedTime_p);
	void applyMove(MobMoveInfo & info_p);
};
