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
	, _labelLife(new RichLabelVessel())
	, _labelTime(new RichLabelVessel())
	, _labelResource(new RichLabelVessel())
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	graphic_l.registerMessage(new NewRichLabelMessage(*_labelLife, getLifeContent(_engine), 0, 0, 600, 40, 10, true));
	graphic_l.registerMessage(new NewRichLabelMessage(*_labelTime, getTimeContent(_engine), 0, 40, 500, 40, 10, true));
	graphic_l.registerMessage(new NewRichLabelMessage(*_labelResource, getScrapContent(_engine), 0, 80, 400, 40, 10, true));
}

HeaderUI::~HeaderUI()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();
	graphic_l.registerMessage(new DestroyWindowMessage(_labelLife->label->getWindow()));
	graphic_l.registerMessage(new DestroyWindowMessage(_labelTime->label->getWindow()));
	graphic_l.registerMessage(new DestroyWindowMessage(_labelResource->label->getWindow()));
	delete _labelLife;
	delete _labelTime;
	delete _labelResource;
}

void HeaderUI::update()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	/// update time
	graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_labelLife, getLifeContent(_engine)));
	// update life
	graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_labelTime, getTimeContent(_engine)));
	// update scrap
	graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_labelResource, getScrapContent(_engine)));
}
