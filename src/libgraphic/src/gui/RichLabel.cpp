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

	_label = _manager->createWidget<Colibri::Label>(_window);
	_label->m_minSize = Ogre::Vector2( width, 0 );
	_label->sizeToFit(width);
	_label->setDefaultFontSize(size_p);
	layout_l->addCell(_label);

	updateText(content_p);


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

void RichLabel::updateText(std::vector<InfoLabel> const &content_p)
{
	std::stringstream ss_l;
	for(InfoLabel const &info_l : content_p)
	{
		ss_l<<info_l.text;
	}

	_label->setText(ss_l.str());

	// add rich text
	Colibri::RichTextVec richText_p;
	size_t offset_l = 0;
	for(InfoLabel const &info_l : content_p)
	{
		Colibri::RichText tmp_l = _label->getDefaultRichText();
		tmp_l.offset = offset_l;
		tmp_l.length = info_l.text.size();
		richText_p.push_back(tmp_l);
		offset_l += info_l.text.size();
	}
	_label->setRichText(richText_p, true);

	// Handle color
	size_t idx_l = 0;
	for(InfoLabel const &info_l : content_p)
	{
		_label->setTextColour(Ogre::ColourValue(info_l.r/255.f, info_l.g/255.f, info_l.b/255.f), idx_l);
		++idx_l;
	}
}
