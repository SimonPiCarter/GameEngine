#include "HeaderUI.h"

#include <string>
#include <sstream>
#include <vector>

#include "logic/utils/Translator.h"
#include "logic/LogicEngine.h"
#include "game/CellsEngine.h"

#include <gui/RichLabel.h>

std::vector<InfoLabel> getLifeContent(LogicEngine &engine_p)
{
	std::vector<InfoLabel> content_l;
	content_l.push_back({translate("Temperature energy level : "), 255, 255, 255});
	std::stringstream ss_l;
	ss_l<<int(engine_p.getLife())<<"%";
	content_l.push_back({translate(ss_l.str()), 0, 255, 255});
	return content_l;
}

std::vector<InfoLabel> getTimeContent(LogicEngine &engine_p)
{
	size_t roundedSecond_l = size_t(engine_p.getTime());
	size_t secondWithoutMinutes_l = roundedSecond_l%60;
	size_t minutes_l = (roundedSecond_l - secondWithoutMinutes_l) / 60;
	std::stringstream ss_l;
	if(minutes_l < 10)
	{
		ss_l << "0";
	}
	ss_l << minutes_l
	     << ":";
	if(secondWithoutMinutes_l < 10)
	{
		ss_l << "0";
	}
	ss_l << secondWithoutMinutes_l;

	std::vector<InfoLabel> content_l;
	content_l.push_back({translate("Time   : "), 255, 255, 255});
	content_l.push_back({translate(ss_l.str()), 0, 255, 255});
	return content_l;
}

std::vector<InfoLabel> getScrapContent(LogicEngine &engine_p)
{
	std::vector<InfoLabel> content_l;
	content_l.push_back({translate("Scraps : "), 255, 255, 255});
	std::stringstream ss_l;
	ss_l<<int(engine_p.getScrap());
	content_l.push_back({translate(ss_l.str()), 0, 255, 255});
	return content_l;
}

HeaderUI::HeaderUI(LogicEngine &engine_p)
	: _engine(engine_p)
	, _labelLife(nullptr)
	, _labelTime(nullptr)
	, _labelResource(nullptr)
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	_labelLife = new RichLabel(getLifeContent(_engine), 0, 0, 600, 40, 10, true, graphic_l);
	_labelTime = new RichLabel(getTimeContent(_engine), 0, 40, 500, 40, 10, true, graphic_l);
	_labelResource = new RichLabel(getScrapContent(_engine), 0, 80, 400, 40, 10, true, graphic_l);
}

HeaderUI::~HeaderUI()
{}

void HeaderUI::update(double elapsedTime_p)
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	/// update time
	_labelTime->updateText(getTimeContent(_engine));

	// update life
	_labelLife->updateText(getLifeContent(_engine));

	// update scrap
	_labelResource->updateText(getScrapContent(_engine));
}
