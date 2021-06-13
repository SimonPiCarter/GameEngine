#include "SlowSlot.h"

#include <sstream>

#include "logic/utils/Translator.h"
#include "logic/effect/buff/Slow.h"

SlowSlot::SlowSlot(std::string const & id_p, unsigned long lvl_p, double slow_p, double duration_p, unsigned long maxStack_p)
	: EffectMaker(id_p, lvl_p), _slow(slow_p), _duration(duration_p), _maxStack(maxStack_p)
{}
SlowSlot::~SlowSlot()
{}
Effect * SlowSlot::newEffect(MobEntity * target_p, Tower const &, WaveEngine &, double timestamp_p) const
{
	return new Slow(target_p, _id, timestamp_p, _duration, _slow, _maxStack);
}

/// @brief return the skin associated with the modifier
std::string SlowSlot::getSkin() const
{
	return "SlowSlot";
}

namespace
{
	std::string to_str(double val_p)
	{
		std::stringstream ss_l;
		ss_l<<val_p;
		return ss_l.str();
	}
} // namespace


/// @brief return a text description for the modifier
std::vector<InfoLabel> SlowSlot::getDesc() const
{
	std::vector<InfoLabel> content_l;

	content_l.push_back({translate("Slow every damaged enemy by "), 255, 255, 255});
	content_l.push_back({translate(to_str(int(_slow*100))+"%"), 150, 150, 255});
	content_l.push_back({translate(" for a period of  "), 255, 255, 255});
	content_l.push_back({translate(to_str(_duration)), 255, 255, 50});
	content_l.push_back({translate(" seconds (stacks "), 255, 255, 255});
	content_l.push_back({translate(to_str(_maxStack)), 255, 255, 50});
	content_l.push_back({translate(" times)"), 255, 255, 255});

	return content_l;
}
