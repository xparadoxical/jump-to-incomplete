#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(MyLevelSelectLayer, LevelSelectLayer)
{
    $override bool init(int page)
    {
        if (!LevelSelectLayer::init(page))
            return false;
        
        auto jumpButton = JumpButton::create(this, menu_selector(MyLevelSelectLayer::onJumpButton));
        auto arrowsMenu = getChildByID("arrows-menu");

        auto gap = 5.0f;
        auto rightButton = (CCMenuItemSpriteExtra*)arrowsMenu->getChildByID("right-button");
        auto pos = CCPoint(rightButton->getPositionX() - rightButton->getScaledContentWidth() / 2 - gap - jumpButton->getScaledContentWidth() / 2, rightButton->getPositionY());
        jumpButton->setPosition(pos);

        arrowsMenu->addChild(jumpButton);

        return true;
    }

    void onJumpButton(CCObject *sender)
    {
        auto glm = GameLevelManager::sharedState();

        const int mainLevelCount = 22; //TODO get from some manager?
        for (int page = m_scrollLayer->m_page + 1; page < mainLevelCount; page++) //TODO wrap-around
        {
            auto level = glm->getMainLevel(page + 1, true);
            if (level->m_orbCompletion.value() < 100/*%*/)
            {
                m_scrollLayer->instantMoveToPage(page);
                break;
            }
        }
    }
};