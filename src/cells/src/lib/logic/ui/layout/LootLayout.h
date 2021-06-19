#pragma once

#include <list>
#include <string>

class Slot;

struct LootLayout
{
    std::string title;
    std::string text;
    std::list<Slot *> loots;
};
