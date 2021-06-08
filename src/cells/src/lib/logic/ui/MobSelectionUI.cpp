#include "MobSelectionUI.h"

#include <string>
#include <sstream>
#include <vector>

#include "logic/utils/Translator.h"
#include "logic/LogicEngine.h"
#include "logic/entity/MobEntity.h"
#include "game/CellsEngine.h"

#include <gui/RichLabel.h>

namespace
{
	std::vector<InfoLabel> getContent(LogicEngine &engine_p)
	{
		std::vector<InfoLabel> content_l;
		if(!engine_p.getMobSelection())
		{
			content_l.push_back({translate("no selection"), 255, 255, 255});
			return content_l;
		}
		MobEntity * entity_l = engine_p.getMobSelection();

		std::stringstream ss_l;
		ss_l<<translate("life        : ")<<int(entity_l->getHitpoint())<<"/"<<int(entity_l->getModel()->hitpoint)<<"\n";
		ss_l<<translate("damage mult : ")<<entity_l->getDamageMutliplier()<<"\n";
		ss_l<<translate("speed       : ")<<entity_l->getSpeed()<<"\n";
		ss_l<<translate("armor type  : ")<<entity_l->getArmorType()<<"\n";

		content_l.push_back({translate(ss_l.str()), 255, 255, 255});
		return content_l;
	}
} // namespace

MobSelectionUI::MobSelectionUI(LogicEngine &engine_p)
	: _engine(engine_p)
	, _label(new RichLabelVessel())
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	int width_l = 500;
	int height_l = 130;
	graphic_l.registerMessage(new NewRichLabelMessage(*_label, getContent(_engine),
		graphic_l.getColibriManager()->getCanvasSize().x - width_l,
		graphic_l.getColibriManager()->getCanvasSize().y - height_l, width_l, height_l, 10, true));
	graphic_l.registerMessage(new HideRichLabelMessage(*_label, true));
}

MobSelectionUI::~MobSelectionUI()
{
	delete _label;
}

void MobSelectionUI::update()
{
	GraphicEngine & graphic_l = _engine.getCellsEngine()->getGraphic();

	if(!_engine.getMobSelection())
	{
		graphic_l.registerMessage(new HideRichLabelMessage(*_label, true));
	}
	else
	{
		graphic_l.registerMessage(new HideRichLabelMessage(*_label, false));
		/// update
		graphic_l.registerMessage(new UpdateTextRichLabelMessage(*_label, getContent(_engine)));
	}
}
