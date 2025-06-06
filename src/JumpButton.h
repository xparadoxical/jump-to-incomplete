#pragma once
#include <Geode/Geode.hpp>

namespace JumpButton
{
    CCMenuItemSpriteExtra* create(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale = 1.0f);

    extern std::string const& id;
}