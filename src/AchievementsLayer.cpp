#include <Geode/Geode.hpp>
#include <Geode/modify/AchievementsLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(JtiAchievementsLayer, AchievementsLayer)
{
    $override void customSetup()
    {
        AchievementsLayer::customSetup();

        auto jumpButton = JumpButton::create(this, menu_selector(JtiAchievementsLayer::onJumpButton), 0.85f);
        //origin is at the bottom-left corner of the screen
        //menu with buttons is scene-sized, bottom-left is at the center

        //button pos calculation
        //SpriteExtras have anchor in the center
        auto gap = 5.0f;
        auto pos = CCPoint(m_nextPageButton->getPositionX() - m_nextPageButton->getScaledContentWidth() / 2 - gap - jumpButton->getScaledContentWidth() / 2, m_nextPageButton->getPositionY());
        jumpButton->setPosition(pos);

        m_nextPageButton->getParent()->addChild(jumpButton);
    }

    $override void loadPage(int page)
    {
        AchievementsLayer::loadPage(page);

        auto jumpButton = (CCMenuItemSpriteExtra*)m_nextPageButton->getParent()->getChildByID(JumpButton::id);
        if (!jumpButton)
        {
            //we're in JtiAchievementsLayer::customSetup->AchievementsLayer::customSetup->JtiAchievementsLayer::loadPage
            //jumpButton doesn't exist yet, it will be visible by default
            return;
        }

        jumpButton->setVisible(m_nextPageButton->isVisible());
    }

    void onJumpButton(CCObject *sender)
    {
        auto am = AchievementManager::sharedState();

        auto achievements = CCArrayExt<CCDictionary*>(am->m_allAchievementsSorted);
        const int pageSize = 10;
        for (int i = pageSize * (m_currentPage + 1); i < achievements.size(); i++)
        {
            auto achId = achievements[i]->valueForKey("identifier")->getCString();
            if (!am->isAchievementEarned(achId))
            {
                auto page = i / pageSize;
                this->loadPage(page);
                //TODO android-only this->m_listLayer->m_listView->m_tableView->scrollToIndexPath(getAchTblVwCell->m_indexPath);
                break;
            }
        }
    }
};