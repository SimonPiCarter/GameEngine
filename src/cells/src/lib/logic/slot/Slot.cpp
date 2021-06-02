#include "Slot.h"

Slot::Slot() {}
Slot::~Slot() {}

EffectMaker::EffectMaker() {}
EffectMaker::~EffectMaker() {}

/// @brief return true iif Slot is of type EffectMaker
bool EffectMaker::isEffectMaker() const { return true; }
/// @brief return true iif Slot is of type Buff
bool EffectMaker::isBuffMaker() const { return false; }
