#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(JtiGauntletSelectLayer, GauntletSelectLayer)
{
    struct Fields
    {
        bool waitForLoad;
    };

    $override bool init(int p0)
    {
        if (!GauntletSelectLayer::init(p0))
            return false;

        if (GameLevelManager::sharedState()->m_savedGauntlets->count() == 0)
        {
            m_fields->waitForLoad = true;
            return true;
        }

        setupJumpButton();

        return true;
    }

    $override void loadLevelsFinished(CCArray* p0, char const* p1, int p2)
    {
        GauntletSelectLayer::loadLevelsFinished(p0, p1, p2);

        if (m_fields->waitForLoad)
            setupJumpButton();
    }

    void setupJumpButton()
    {
        auto jumpButton = JumpButton::create(this, menu_selector(JtiGauntletSelectLayer::onJumpButton));

        auto scrollButtonsMenu = getChildByID("scroll-buttons-menu");
        auto rightButton = scrollButtonsMenu->getChildByID("right-button");

        auto gap = 5.0f;
        auto pos = CCPoint(rightButton->getPositionX() - rightButton->getScaledContentWidth() / 2 - gap - jumpButton->getScaledContentWidth() / 2, rightButton->getPositionY());
        jumpButton->setPosition(pos);

        scrollButtonsMenu->addChild(jumpButton);
        m_fields->waitForLoad = false;
    }

    void onJumpButton(CCObject *sender)
    {
        auto glm = GameLevelManager::sharedState();

        auto gauntletIds = glm->m_savedGauntlets->allKeys();
        auto startingPage = m_scrollLayer->m_page;
        auto pageCount = m_scrollLayer->getTotalPages();
        auto totalItems = glm->m_savedGauntlets->count();
        log::debug("pageCount {}, items {}", pageCount, totalItems);

        const int pageSize = 3;
        for (int i = 0; i < pageCount; i++) //check the next pageCount pages
        {
            auto pageIndex = (startingPage + 1/*start at next page*/ + i) % pageCount;
            auto pageItemCount = std::min(pageSize, (int)totalItems - (int)pageIndex * pageSize);
            log::debug("checking page {} with {} items", pageIndex, pageItemCount);

            for (int j = 0; j < pageItemCount; j++) //check all items on the page
            {
                auto itemIndex = pageIndex * pageSize + j;

                auto id = gauntletIds->stringAtIndex(itemIndex);
                auto gauntlet = (GJMapPack*)glm->m_savedGauntlets->objectForKey(id->getCString());
                if (!gauntlet->hasCompletedMapPack())
                {
                    log::debug("jumping to page {}", pageIndex);
                    m_scrollLayer->moveToPage(pageIndex);
                    return;
                }
            }
        }

        log::debug("no item found");
        ((CCNode*)sender)->setVisible(false);
    }
};