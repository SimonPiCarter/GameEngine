#pragma once

#include <array>

#include "ColibriGui/ColibriManager.h"

class Slot;
struct RichLabelVessel;

struct SlotUI
{
	Colibri::Button * _button = nullptr;
	Colibri::Renderable * _image = nullptr;
	Colibri::Renderable * _select = nullptr;
	Colibri::Renderable * _disabled = nullptr;
	Colibri::Label * _label = nullptr;
	Slot * _slot = nullptr;
	RichLabelVessel * _tooltip = nullptr;
};

SlotUI * createSlotUI(Colibri::ColibriManager * manager_p, Colibri::Window * window_p, std::array<double, 2> pos_p, unsigned long size_p);

void updateSlotUI(SlotUI &slotUI_p, Slot * slot_p);
