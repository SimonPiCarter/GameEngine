#include "WaveEngine.h"

#include <chrono>
#include <cassert>

#include "LogicEngine.h"
#include "layout/MobModelLayout.h"
#include "wave/MobSpawner.h"

WaveEngine::WaveEngine(LogicEngine &logic_p)
	: _logic(logic_p)
	, _tree({{0.,0.}, {double(_logic._currentMap->getSize()[0]), double(_logic._currentMap->getSize()[1])}}, 20)
	, _spawner(nullptr)
{}

WaveEngine::~WaveEngine()
{
	for(MobEntity * ent_l : _mobs)
	{
		delete ent_l;
	}
	delete _spawner;
}

void WaveEngine::waveLoop(WaveLayout const &layout_p)
{
	std::chrono::time_point<std::chrono::system_clock> start_l = std::chrono::system_clock::now();
	double timeSinceLast_l = 1.0 / 60.0;

	delete _spawner;
	_spawner = new MobSpawner(*this, layout_p, *_logic._currentMap);
	_spawnOver = false;

	while(!_logic._quit && !isWaveOver())
	{
		handleFrame(timeSinceLast_l);

		std::chrono::time_point<std::chrono::system_clock> end_l = std::chrono::system_clock::now();
		timeSinceLast_l = (end_l - start_l) / std::chrono::milliseconds(1);
		timeSinceLast_l = std::min( 1.0, timeSinceLast_l/1000. ); //Prevent from going haywire.
		start_l = end_l;
	}
}

void WaveEngine::handleFrame(double elapsedTime_p)
{
	assert(_spawner);
	// Spawn mob
	if(!_spawnOver)
	{
		_spawnOver = _spawner->spawn(elapsedTime_p);
	}

	// Compute mob movement

	// Apply movement and update octrees

	// Deduce life

	// Trigger attack

	// Resolve effects

}


void WaveEngine::spawnMob(MobModel const &model_p, std::array<double, 2> const & spawnPosition_p)
{
	std::array<double, 2> pos_l = {
		spawnPosition_p[0] - model_p.size[0]/2.,
		spawnPosition_p[1] - model_p.size[1]/2.
	};
	assert(_logic._currentMap);
	_mobs.push_back(new MobEntity(pos_l, &model_p, *_logic._currentMap));
	_tree.addContent(_mobs.back());
}

bool WaveEngine::isWaveOver()
{
	return _spawnOver && _mobs.empty();
}
