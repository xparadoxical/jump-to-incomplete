#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>
#include "JumpButton.h"
#include "utils.h"
#include "scroll.h"

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

    void onJumpButton(CCObject* sender)
    {
        auto glm = GameLevelManager::sharedState();

        const int pageSize = 3;
        auto totalItems = glm->m_savedGauntlets->count();
        auto gauntletIds = glm->m_savedGauntlets->allKeys();

        jti::scroll::toIncompletePage(m_scrollLayer, pageSize, totalItems,
            [gauntletIds, glm](int pageIndex, int itemIndex) -> bool {
                auto id = gauntletIds->stringAtIndex(itemIndex);
                auto gauntlet = (GJMapPack*)glm->m_savedGauntlets->objectForKey(id->getCString());
                return gauntlet->hasCompletedMapPack();
            },
            jti::scroll::jumpForward, (CCNode*)sender);
    }
};