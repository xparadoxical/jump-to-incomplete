#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(MyGauntletSelectLayer, GauntletSelectLayer)
{
    $override bool init(int p0)
    {
        if (!GauntletSelectLayer::init(p0))
            return false;

        return true;
    }
    
    $override void setupGauntlets() //at this point savedGauntlets is valid
    {
        GauntletSelectLayer::setupGauntlets();

        auto glm = GameLevelManager::sharedState();
            
        auto gauntletIds = glm->m_savedGauntlets->allKeys();
        const int pageSize = 3;
        for (int i = pageSize * (m_scrollLayer->m_page + 1); i < gauntletIds->count(); i++)
        {
            auto id = gauntletIds->stringAtIndex(i);
            auto gauntlet = (GJMapPack*)glm->m_savedGauntlets->objectForKey(id->getCString());
            if (!gauntlet->hasCompletedMapPack())
            {
                m_scrollLayer->moveToPage(i / pageSize);
                break;
            }
        }
    }
};