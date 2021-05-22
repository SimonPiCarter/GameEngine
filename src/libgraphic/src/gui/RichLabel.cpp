#include "RichLabel.h"

#include "engine/GraphicEngine.h"

#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/Layouts/ColibriLayoutLine.h"

#include <iostream>
#include <sstream>

RichLabel::RichLabel(std::vector<InfoLabel> const &content_p, double x, double y, double width, double height, float size_p, GraphicEngine &graphic_p)
{
	_manager = graphic_p.getColibriManager();
	_window = _manager->createWindow(nullptr);

	// Adjust window to the full
	_window->setTransform(Ogre::Vector2(x, y-20), Ogre::Vector2(width, height));
	_window->m_breadthFirst = true;
	_window->setVisualsEnabled(false);

	Colibri::LayoutLine *layout_l = new Colibri::LayoutLine(_manager);
	layout_l->setCellOffset(Ogre::Vector2(0,0));

	std::stringstream ss_l;
	for(InfoLabel const &info_l : content_p)
	{
		ss_l<<info_l.text;
	}

	Colibri::Label *label_l = _manager->createWidget<Colibri::Label>(_window);
	label_l->m_minSize = Ogre::Vector2( width, 0 );
	label_l->setDefaultFontSize(size_p);
	label_l->setText(ss_l.str());
	label_l->sizeToFit(width);
	layout_l->addCell(label_l);

	// add rich text
	Colibri::RichTextVec richText_p;
	size_t offset_l = 0;
	for(InfoLabel const &info_l : content_p)
	{
		Colibri::RichText tmp_l = label_l->getDefaultRichText();
		tmp_l.offset = offset_l;
		tmp_l.length = info_l.text.size();
		richText_p.push_back(tmp_l);
		offset_l += info_l.text.size();
	}
	label_l->setRichText(richText_p, true);

	// Handle color
	size_t idx_l = 0;
	for(InfoLabel const &info_l : content_p)
	{
		label_l->setTextColour(Ogre::ColourValue(info_l.r/255.f, info_l.g/255.f, info_l.b/255.f), idx_l);
		++idx_l;
	}

	layout_l->setAdjustableWindow(_window);
	layout_l->m_hardMaxSize = Ogre::Vector2(width, height+(40*size_p/10));
	layout_l->layout();

	// no scroll here!
	_window->sizeScrollToFit();
	_window->setMaxScroll(Ogre::Vector2(0,0));
	_window->setScrollableArea(Ogre::Vector2(0,0));
}

RichLabel::~RichLabel()
{
	_manager->destroyWidget(_window);
}
