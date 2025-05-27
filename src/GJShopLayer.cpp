#include <Geode/Geode.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

void debugDict(CCDictionary* dict);

class $modify(MyGJShopLayer, GJShopLayer)
{
    $override bool init(ShopType p0)
    {
        if (!GJShopLayer::init(p0))
            return false;

        auto gsm = GameStatsManager::sharedState();

        auto itemIndexes = CCArrayExt<CCInteger*>(m_shopItems->allKeys());
        for (int i = 8; i < itemIndexes.size(); i++)
        {
            if (!gsm->isStoreItemUnlocked(itemIndexes[i]->getValue()))
            {
                getChildByType<ListButtonBar*>(0)->goToPage(i / 8);
                break;
            }
        }

        return true;
    }
};