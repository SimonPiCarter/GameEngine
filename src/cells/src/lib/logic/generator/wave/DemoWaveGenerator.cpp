#include "DemoWaveGenerator.h"

#include "logic/slot/effect_maker/SlowSlot.h"
#include "logic/slot/AttackModifier.h"

DemoWaveGenerator::DemoWaveGenerator(WaveLayout const & layout_p)
	: WaveGenerator(0)
	, _layout(layout_p)
{}

WaveLayout * DemoWaveGenerator::nextWave()
{
	WaveLayout * newLayout_l = new WaveLayout(_layout);
	newLayout_l->rewards.push_back(new SlowSlot("SlowTest", 120, 0.2, 15., 4));
	newLayout_l->rewards.push_back(new AttackModifier("special", 27, 70 , 2., 2., 3., AttackType::Splash, DamageType::Standard));
	return newLayout_l;
}
