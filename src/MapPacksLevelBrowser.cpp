#include <Geode/Geode.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(MyLevelBrowserLayer, LevelBrowserLayer)
{
    struct Fields
    {
        CCMenuItemSpriteExtra* jumpButton;
        bool searching;
    };

    $override bool init(GJSearchObject* p0) {
        if (!LevelBrowserLayer::init(p0))
            return false;

        if (m_searchObject->m_searchType != SearchType::MapPack)
            return true;

        auto jumpButton = JumpButton::create(this, menu_selector(MyLevelBrowserLayer::onJumpButton), 0.85f);
        m_fields->jumpButton = jumpButton;

        auto nextPageMenu = getChildByID("next-page-menu");
        nextPageMenu->getLayout()->ignoreInvisibleChildren(true); //just in case there'll be more than 2 buttons
        ((AxisLayout*)nextPageMenu->getLayout())->setAutoScale(false);
        nextPageMenu->addChild(jumpButton);
        updateVisibility();

        return true;
    }

    void updateVisibility()
    {
        if (auto jumpButton = m_fields->jumpButton)
        {
            jumpButton->setVisible(m_rightArrow->isVisible());
            jumpButton->getParent()->updateLayout();
        }
    }

    void searchCurrentPage()
    {
        auto mapPacks = CCArrayExt<GJMapPack*>(m_list->m_listView->m_entries);
        for (auto pack : mapPacks)
        {
            if (!pack->hasCompletedMapPack())
            {
                m_fields->searching = false;
                //TODO android-only this->m_listLayer->m_listView->m_tableView->scrollToIndexPath(getAchTblVwCell->m_indexPath);
                //and m_fields->foundAndScrolled = true;
            }
        }

        if (!m_rightArrow->isVisible())
            m_fields->searching = false;
    }

    $override void loadPage(GJSearchObject* p0)
    {
        //m_fields->foundAndScrolled = false;
        LevelBrowserLayer::loadPage(p0);

        updateVisibility();
    }

    $override void loadLevelsFinished(CCArray* entries, char const* key, int p2)
    {
        LevelBrowserLayer::loadLevelsFinished(entries, key, p2);

        updateVisibility();

        searchCurrentPage();
        if (m_fields->searching)
            onNextPage(m_rightArrow);
    }

    $override void loadLevelsFailed(char const* p0, int p1)
    {
        LevelBrowserLayer::loadLevelsFailed(p0, p1);

        m_fields->searching = false;
    }

    void onJumpButton(CCObject *sender)
    {
        //TODO
        //if (!m_fields->foundAndScrolled)
        //    searchCurrentPage();
        //then return if not searching

        m_fields->searching = true;
        onNextPage(m_rightArrow);
    }
};