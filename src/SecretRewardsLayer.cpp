#include <Geode/Geode.hpp>
#include <Geode/modify/SecretRewardsLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(MySecretRewardsLayer, SecretRewardsLayer)
{
    $override bool init(bool p0)
    {
        if (!SecretRewardsLayer::init(p0))
            return false;

        return true;
    }

    $override void onChestType(CCObject* sender)
    {
        SecretRewardsLayer::onChestType(sender);

        //not gonna decipher what SecretRewardsLayer::generateChestItems does
        auto chestsLayer = m_secondaryScrollLayer->m_extendedLayer;
        auto pageLayers = CCArrayExt<CCLayer*>(chestsLayer->getChildren());
        for (int page = m_secondaryScrollLayer->m_page + 1; page < pageLayers.size(); page++)
        {
            for (auto chestSprite : CCArrayExt<CCMenuItemSpriteExtra*>(pageLayers[page]->getChildByType<CCMenu*>(0)->getChildren()))
            {
                auto id = chestSprite->getTag();
                if (!GameStatsManager::sharedState()->isSecretChestUnlocked(id))
                {
                    m_secondaryScrollLayer->moveToPage(page);
                    return;
                }
            }
        }
    }

    void onJumpButton(CCObject *sender)
    {
        log::info("SecretRewardsLayer::onJumpButton");
    }
};