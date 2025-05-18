#include <Geode/Geode.hpp>
#include <Geode/modify/AchievementsLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(MyAchievementsLayer, AchievementsLayer)
{
    $override void customSetup()
    {
        AchievementsLayer::customSetup();
        log::debug("AchievementsLayer.customSetup()");

        auto jumpButton = JumpButton::create(this, menu_selector(MyAchievementsLayer::onJumpButton), 0.75f);

        auto nextPageButton = this->m_nextPageButton;
        auto originalMenu = nextPageButton->getParent(); //scene-sized, bottom-left is at the center
        if (originalMenu->getLayout())
            originalMenu->addChild(jumpButton);
        else
        {
            //button pos calculation
            auto gap = 5.0f;
            auto bottomCenterAbsolute = originalMenu->getPosition() + nextPageButton->getPosition() - CCPoint(0.0f, nextPageButton->getContentWidth() / 2 + gap/*compensation for the layout acting weird*/);

            nextPageButton->removeFromParentAndCleanup(false);

            auto buttons = CCMenu::create();
            buttons->setID("next-page-menu");
            this->m_mainLayer->addChild(buttons);

            buttons->setAnchorPoint(CCPoint(0.5f, 0.0f));
            buttons->setPosition(bottomCenterAbsolute);
            buttons->setLayout(SimpleColumnLayout::create()
                ->setMainAxisScaling(AxisScaling::Fit)
                ->setCrossAxisScaling(AxisScaling::Fit)
                ->setMainAxisAlignment(MainAxisAlignment::Start)
                ->setMainAxisDirection(AxisDirection::BottomToTop)
                ->setGap(gap));
            buttons->addChild(nextPageButton);
            buttons->addChild(jumpButton);
            buttons->updateLayout();
        }
    }

    void onJumpButton(CCObject *sender)
    {
        auto am = AchievementManager::sharedState();

        auto achievements = CCArrayExt<CCDictionary*>(am->m_allAchievementsSorted);
        for (int i = 10 * (m_currentPage + 1) /*start at next page*/; i < achievements.size(); i++)
        {
            auto achId = achievements[i]->valueForKey("identifier")->getCString();
            auto completed = am->isAchievementEarned(achId);

            if (!completed)
            {
                auto page = i / 10;
                this->loadPage(page);
                //TODO android-only this->m_listLayer->m_listView->m_tableView->scrollToIndexPath(getAchTblVwCell->m_indexPath);
                break;
            }
        }
    }
};