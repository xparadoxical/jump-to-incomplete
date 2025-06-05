#include <Geode/Geode.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include "JumpButton.h"
#include "utils.h"
#include "scroll.h"

using namespace geode::prelude;

class $modify(JtiGJShopLayer, GJShopLayer)
{
    $override bool init(ShopType p0)
    {
        if (!GJShopLayer::init(p0))
            return false;

        auto jumpButton = JumpButton::create(this, menu_selector(JtiGJShopLayer::onJumpButton), 0.75f);

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

    void onJumpButton(CCObject* sender)
    {
        auto gsm = GameStatsManager::sharedState();

        const int pageSize = 8;
        auto shopItemIndexes = CCArrayExt<CCInteger*>(m_shopItems->allKeys());
        auto totalItems = shopItemIndexes.size();

        jti::scroll::toIncompletePage(getChildByType<ListButtonBar*>(0)->m_scrollLayer, pageSize, totalItems,
            [&shopItemIndexes, &gsm](int pageIndex, int itemIndex) {
                return gsm->isStoreItemUnlocked(shopItemIndexes[itemIndex]->getValue());
            },
            jti::scroll::jumpForward, (CCNode*)sender);
    }
};