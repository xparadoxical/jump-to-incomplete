#include <Geode/Geode.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(MyGJShopLayer, GJShopLayer)
{
    $override bool init(ShopType p0)
    {
        if (!GJShopLayer::init(p0))
            return false;

        auto jumpButton = JumpButton::create(this, menu_selector(MyGJShopLayer::onJumpButton), 0.75f);

        auto navigationMenu = getChildByType<ListButtonBar>(0)->getChildByType<CCMenu>(0);
        auto rightButton = getRightButton(navigationMenu);

        auto gap = 5.0f;
        auto pos = CCPoint(rightButton->getPositionX(), rightButton->getPositionY() + rightButton->getScaledContentHeight() / 2 + gap + jumpButton->getScaledContentHeight() / 2);
        jumpButton->setPosition(pos);

        navigationMenu->addChild(jumpButton);

        return true;
    }

    CCMenuItemSpriteExtra* getRightButton(CCMenu* menu) //pain
    {
        for (auto child : CCArrayExt<CCNode*>(menu->getChildren()))
        {
            if (auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(child))
            {
                if (button->getChildByType<CCSprite*>(0)->isFlipX())
                    return button;
            }
        }

        return nullptr;
    }

    void onJumpButton(CCObject *sender)
    {
        auto gsm = GameStatsManager::sharedState();

        auto list = getChildByType<ListButtonBar*>(0);
        auto itemIndexes = CCArrayExt<CCInteger*>(m_shopItems->allKeys());
        const int pageSize = 8;
        for (int i = pageSize * (list->m_scrollLayer->m_page + 1); i < itemIndexes.size(); i++)
        {
            if (!gsm->isStoreItemUnlocked(itemIndexes[i]->getValue()))
            {
                list->goToPage(i / pageSize);
                break;
            }
        }
    }
};