#include <Geode/Geode.hpp>
#include <Geode/modify/SecretRewardsLayer.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

class $modify(MySecretRewardsLayer, SecretRewardsLayer)
{
    $override void onChestType(CCObject* sender)
    {
        SecretRewardsLayer::onChestType(sender);

        auto jumpButton = JumpButton::create(this, menu_selector(MySecretRewardsLayer::onJumpButton), 0.85f);
        auto gap = 5.0f;
        auto pos = CCPoint(m_rightButton->getPositionX() - m_rightButton->getScaledContentWidth() / 2 - gap - jumpButton->getScaledContentWidth() / 2, m_rightButton->getPositionY());
        auto worldPos = m_rightButton->getParent()->convertToWorldSpace(pos);
        jumpButton->setPosition(worldPos);

        auto jumpButtonMenu = CCMenu::createWithItem(jumpButton);
        jumpButtonMenu->setID(JumpButton::id + "-menu");
        jumpButtonMenu->setPosition({0, 0});
        m_secondaryLayer->addChild(jumpButtonMenu);
    }

    void onJumpButton(CCObject *sender)
    {
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
                    m_secondaryScrollLayer->instantMoveToPage(page);
                    return;
                }
            }
        }
    }
};