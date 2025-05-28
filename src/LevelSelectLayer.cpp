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
        
        auto glm = GameLevelManager::sharedState();

        const int mainLevelCount = 22;
        for (int page = m_scrollLayer->m_page + 1; page < mainLevelCount; page++)
        {
            auto level = glm->getMainLevel(page + 1, true);
            if (level->m_orbCompletion.value() < 100/*%*/)
            {
                m_scrollLayer->instantMoveToPage(page);
                break;
            }
        }
        
        return true;
    }
};