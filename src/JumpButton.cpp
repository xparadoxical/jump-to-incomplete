#include "JumpButton.h"
using namespace geode::prelude;

CCMenuItemSpriteExtra* JumpButton::create(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale)
{
    auto sprite = CCMenuItemSpriteExtra::create(CCSprite::create("jump-button.png"_spr), target, callback);
    sprite->m_baseScale = scale; //sets onmouseup target scale
    sprite->setScale(sprite->m_baseScale); //sets initial scale
    return sprite;
}