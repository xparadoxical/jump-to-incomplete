#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include "JumpButton.h"
#include "utils.h"

using namespace geode::prelude;

class $modify(JtiLevelSelectLayer, LevelSelectLayer)
{
    $override bool init(int page)
    {
        if (!LevelSelectLayer::init(page))
            return false;
        
        auto jumpButton = JumpButton::create(this, menu_selector(JtiLevelSelectLayer::onJumpButton));
        auto arrowsMenu = getChildByID("arrows-menu");

        auto gap = 5.0f;
        auto rightButton = (CCMenuItemSpriteExtra*)arrowsMenu->getChildByID("right-button");
        auto pos = CCPoint(rightButton->getPositionX() - rightButton->getScaledContentWidth() / 2 - gap - jumpButton->getScaledContentWidth() / 2, rightButton->getPositionY());
        jumpButton->setPosition(pos);

        arrowsMenu->addChild(jumpButton);

        return true;
    }

    void onJumpButton(CCObject* sender)
    {
        auto glm = GameLevelManager::sharedState();

        auto pageCount = m_scrollLayer->getTotalPages();
        auto startingPage = jti::utils::floorMod(m_scrollLayer->m_page, pageCount);
        log::debug("pageCount {}", pageCount);

        const int mainLevelCount = 22; //TODO get from some manager?
        //TODO levelIndex 0 is checked 3 times - for pages 22,23,0
        for (int i = 0; i < pageCount; i++) //check the next pageCount pages
        {
            auto boundedPage = (startingPage + i) % pageCount;
            auto boundedLevelPage = std::min(mainLevelCount - 1, boundedPage); //ignore last non-level pages
            auto nextLevelPage = (boundedLevelPage + 1) % mainLevelCount;

            log::debug("page {}, level page {}, next level {}", boundedPage, boundedLevelPage, nextLevelPage);

            auto level = glm->getMainLevel(nextLevelPage + 1, true);
            if (level->m_orbCompletion.value() < 100/*%*/)
            {
                log::debug("jumping to page {}", nextLevelPage);
                m_scrollLayer->instantMoveToPage(nextLevelPage);
                return;
            }
        }

        log::debug("no item found");
        ((CCNode*)sender)->setVisible(false);
    }
};