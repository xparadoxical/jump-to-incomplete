#include "JumpButton.h"
using namespace geode::prelude;

CCMenuItemSpriteExtra* JumpButton::create(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback)
{
    return CCMenuItemSpriteExtra::create(CCSprite::create("jump-button.png"_spr), target, callback);
}