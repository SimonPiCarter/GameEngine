#include "WaveEngine.h"

#include <chrono>

#include "LogicEngine.h"

WaveEngine::WaveEngine(LogicEngine &logic_p) : _logic(logic_p)
{}

void WaveEngine::waveLoop(WaveLayout const &layout_p)
{

	std::chrono::time_point<std::chrono::system_clock> start_l = std::chrono::system_clock::now();
	double timeSinceLast_l = 1.0 / 60.0;

	while(!_logic._quit && !isWaveOver())
	{
		// Spawn mob
		// TODO

		// Compute mob movement

		// Apply movement and update octrees

		// Deduce life

		// Trigger attack

		// Resolve effects



		std::chrono::time_point<std::chrono::system_clock> end_l = std::chrono::system_clock::now();
		timeSinceLast_l = (end_l - start_l) / std::chrono::milliseconds(1);
		timeSinceLast_l = std::min( 1.0, timeSinceLast_l/1000. ); //Prevent from going haywire.
		start_l = end_l;
	}
}
