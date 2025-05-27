#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

void debugDict(CCDictionary* dict);

#define MainLevelCount 22

class $modify(MyLevelSelectLayer, LevelSelectLayer)
{
    $override bool init(int page)
    {
        if (!LevelSelectLayer::init(page))
            return false;
        
        auto glm = GameLevelManager::sharedState();

        for (int i = m_scrollLayer->m_page + 1; i < MainLevelCount; i++)
        {
            auto level = glm->getMainLevel(i + 1, true);
            if (level->m_orbCompletion.value() < 100)
            {
                m_scrollLayer->moveToPage(i);
                break;
            }
        }
        
        return true;
    }
};