#include <Geode/Geode.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(MyGJShopLayer, GJShopLayer)
{
    $override bool init(ShopType p0)
    {
        if (!GJShopLayer::init(p0))
            return false;

        auto gsm = GameStatsManager::sharedState();

        auto list = getChildByType<ListButtonBar*>(0);
        auto itemIndexes = CCArrayExt<CCInteger*>(m_shopItems->allKeys());
        const int pageSize = 8;
        for (int i = pageSize * (list->m_scrollLayer->m_page + 1); i < itemIndexes.size(); i++)
        {
            if (!gsm->isStoreItemUnlocked(itemIndexes[i]->getValue()))
            {
                list->goToPage(i / pageSize);
                break;
            }
        }

        return true;
    }
};