#include "MobMover.h"

#include "logic/entity/MobEntity.h"
#include "logic/utils/ArrayUtils.h"
#include "logic/WaveEngine.h"

MobMover::MobMover(WaveEngine &waveEngine_p)
	: _waveEngine(waveEngine_p)
{}

void MobMover::moveEntities(std::list<MobEntity *> mobs_p, double elapsedTime_p)
{
	// iterate on every mob to move it
	for(MobEntity * entity_l : mobs_p)
	{
		// get move information and pop checkpoints from entity
		MobMoveInfo info_l = computeMoveInfo(entity_l, elapsedTime_p);

		applyMove(info_l);
	}
}

MobMoveInfo MobMover::computeMoveInfo(MobEntity * entity_p, double elapsedTime_p)
{
	// adjusted elapsed time based on spawn time
	double adjustedElapsedTime_l = std::min(elapsedTime_p, _waveEngine.getTimeStamp() - entity_p->getSpawntime());
	// get checkpoint of entity
	std::list<std::array<double, 2> > & checkPoints_l = entity_p->getCheckpoints();

	// current position
	std::array<double, 2> pos_l = entity_p->getPosition();

	MobMoveInfo info_l;
	info_l.entity = entity_p;
	info_l.pos = pos_l;
	info_l.dir = {1.,0.};
	if(entity_p->getSpeed() < 1e-5)
	{
		return info_l;
	}

	double remainingTime_l = adjustedElapsedTime_l;
	// while there is remaining time
	while(remainingTime_l > 1e-5)
	{
		std::array<double, 2> nextPoint_l = checkPoints_l.front();
		// compute direction to next point
		std::array<double, 2> dir_l = {
			nextPoint_l[0] - pos_l[0],
			nextPoint_l[1] - pos_l[1]
		};

		// square distance
		double dist_l = sqrt(dir_l[0]*dir_l[0] + dir_l[1]*dir_l[1]);
		// compute time to next
		double timeToNext_l = dist_l / entity_p->getSpeed();
		// compute time used to go to next
		double usedTime_l = std::min(remainingTime_l, timeToNext_l);
		// update remaining time
		remainingTime_l -= usedTime_l;

		// normalize now
		dir_l = {dir_l[0]/dist_l, dir_l[1]/dist_l};

		// update position
		pos_l[0] = pos_l[0] + dir_l[0] * usedTime_l * entity_p->getSpeed();
		pos_l[1] = pos_l[1] + dir_l[1] * usedTime_l * entity_p->getSpeed();

		info_l.pos = pos_l;
		info_l.dir = dir_l;

		// if we catched next point we pop it from check points
		if(usedTime_l >= timeToNext_l)
		{
			checkPoints_l.pop_front();
		}
	}

	return info_l;
}

void MobMover::applyMove(MobMoveInfo & info_p)
{
	_waveEngine.moveMob(info_p.entity, info_p.pos, info_p.dir);
}

