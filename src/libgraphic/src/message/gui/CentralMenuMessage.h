#pragma once

#include "message/GraphicMessage.h"
#include "gui/CentralMenu.h"

#include <vector>

struct CentralMenuVessel
{
	central_menu::Menu * menu = nullptr;
};

class NewCentralMenuMessage : public GraphicMessage
{
public:
	NewCentralMenuMessage(CentralMenuVessel & menu_p, std::string const &title_p, std::vector<ButtonData> const &data_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	CentralMenuVessel & getMenu() const { return _menu; }
	std::string const &getTitle() const { return _title; }
	std::vector<ButtonData> const &getData() const { return _data; }

private:
	CentralMenuVessel & _menu;
	std::string const _title;
	std::vector<ButtonData> const _data;
};

class HideCentralMenuMessage : public GraphicMessage
{
public:
	HideCentralMenuMessage(CentralMenuVessel & menu_p, bool hide_p);

	virtual void visit(GraphicMessageHandler &handler_p);

	CentralMenuVessel & getMenu() const { return _menu; }
	bool isHidden() const { return _hidden; }
private:
	CentralMenuVessel & _menu;
	bool const _hidden;
};


