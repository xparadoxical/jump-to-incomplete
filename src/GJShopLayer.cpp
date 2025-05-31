#include <Geode/Geode.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include "JumpButton.h"
#include "utils.h"

using namespace geode::prelude;

class $modify(JtiGJShopLayer, GJShopLayer)
{
    struct Fields
    {
        CCMenuItemSpriteExtra* rightButton;
    };

    $override bool init(ShopType p0)
    {
        if (!GJShopLayer::init(p0))
            return false;

        auto jumpButton = JumpButton::create(this, menu_selector(JtiGJShopLayer::onJumpButton), 0.75f);

        auto navigationMenu = getChildByType<ListButtonBar>(0)->getChildByType<CCMenu>(0);
        auto rightButton = m_fields->rightButton = getRightButton(navigationMenu);

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
        auto list = getChildByType<ListButtonBar*>(0);

        auto shopItemIndexes = CCArrayExt<CCInteger*>(m_shopItems->allKeys());
        auto pageCount = list->m_scrollLayer->getTotalPages();
        auto startingPage = jti::utils::floorMod(list->m_scrollLayer->m_page, pageCount);
        auto totalItems = shopItemIndexes.size();
        log::debug("pageCount {}, items {}", pageCount, totalItems);

        const int pageSize = 8;
        for (int i = 0; i < pageCount; i++) //check the next pageCount pages
        {
            auto pageIndex = (startingPage + 1/*start at next page*/ + i) % pageCount;
            auto pageItemCount = std::min((size_t)pageSize, totalItems - pageIndex * pageSize);
            log::debug("checking page {} with {} items", pageIndex, pageItemCount);

            for (int j = 0; j < pageItemCount; j++) //check all items on the page
            {
                auto itemIndex = pageIndex * pageSize + j;
                if (!gsm->isStoreItemUnlocked(shopItemIndexes[itemIndex]->getValue()))
                {
                    //(instant)moveToPage sometimes results in empty pages
                    log::debug("jumping {} pages forward", i + 1);
                    for (int k = 0; k < i + 1; k++)
                        list->onRight(m_fields->rightButton);
                    return;
                }
            }
        }

        log::debug("no item found");
        ((CCNode*)sender)->setVisible(false);
    }
};