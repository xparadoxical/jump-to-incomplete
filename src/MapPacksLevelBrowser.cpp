#include <Geode/Geode.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(MyLevelBrowserLayer, LevelBrowserLayer)
{
    struct Fields
    {
        bool searching;
    };

    $override void onEnter()
    {
        LevelBrowserLayer::onEnter();

        m_fields->searching = true;
        //TODO when starting search, first scan CCArray<GJMapPack*> m_list->m_listView->m_entries before going to next page
    }

    $override bool init(GJSearchObject* p0) {
        if (!LevelBrowserLayer::init(p0))
            return false;

        return true;
    }

    $override void loadLevelsFinished(CCArray* entries, char const* key, int p2)
    {
        LevelBrowserLayer::loadLevelsFinished(entries, key, p2);

        if (!m_fields->searching)
            return;
        
        auto mapPacks = CCArrayExt<GJMapPack*>(entries);
        for (auto pack : mapPacks)
        {
            if (!pack->hasCompletedMapPack())
            {
                m_fields->searching = false;
                //TODO android-only this->m_listLayer->m_listView->m_tableView->scrollToIndexPath(getAchTblVwCell->m_indexPath);
                return;
            }
        }

        if (!m_rightArrow->isVisible())
        {
            m_fields->searching = false;
            return;
        }

        onNextPage(m_rightArrow);
    }

    $override void loadLevelsFailed(char const* p0, int p1)
    {
        LevelBrowserLayer::loadLevelsFailed(p0, p1);

        m_fields->searching = false;
    }
};