#include "JumpButton.h"
using namespace geode::prelude;

std::string const& JumpButton::id = "jump-button"_spr;

CCMenuItemSpriteExtra* JumpButton::create(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale)
{
    auto sprite = CCMenuItemSpriteExtra::create(CCSprite::create("jump-button.png"_spr), target, callback);
    sprite->m_baseScale = scale; //sets onmouseup target scale
    sprite->setScale(sprite->m_baseScale); //sets initial scale
    sprite->setID(JumpButton::id);
    return sprite;
}