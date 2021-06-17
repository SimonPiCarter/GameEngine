#include "WaveGenerator.h"

#include "logic/layout/WaveLayout.h"

class DemoWaveGenerator : public WaveGenerator
{
public:
	DemoWaveGenerator(WaveLayout const & layout_p);

protected:
	WaveLayout _layout;

	virtual WaveLayout * nextWave() final;
};
