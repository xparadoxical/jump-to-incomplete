#include <Geode/Geode.hpp>
#include <Geode/modify/SecretRewardsLayer.hpp>
#include "JumpButton.h"
#include "utils.h"
#include "scroll.h"

using namespace geode::prelude;

class $modify(JtiSecretRewardsLayer, SecretRewardsLayer)
{
    struct Fields
    {
        GJRewardType chestType;

        int getChestCount()
        {
            switch (chestType)
            {
                case GJRewardType::SmallTreasure: return 400;
                case GJRewardType::LargeTreasure: return 100;
                case GJRewardType::Key10Treasure: return 60;
                case GJRewardType::Key25Treasure: return 24;
                case GJRewardType::Key50Treasure: return 12;
                case GJRewardType::Key100Treasure: return 8;
                default:
                    log::warn("unhandled chest type {}", (int)chestType);
                    return 0;
            }
        }
    };

    $override void createSecondaryLayer(int chestType)
    {
        SecretRewardsLayer::createSecondaryLayer(chestType);

        if (m_secondaryScrollLayer->getTotalPages() == 1)
            return;

        m_fields->chestType = (GJRewardType)chestType;

        auto jumpButton = JumpButton::create(this, menu_selector(JtiSecretRewardsLayer::onJumpButton), 0.85f);
        auto gap = 5.0f;
        //have to account for fSizeMult here because jumpButton will be below rightButton
        auto pos = CCPoint(m_rightButton->getPositionX(), m_rightButton->getPositionY() + m_rightButton->getScaledContentHeight() / 2 * m_rightButton->m_fSizeMult + gap + jumpButton->getScaledContentHeight() / 2);
        auto worldPos = m_rightButton->getParent()->convertToWorldSpace(pos);
        jumpButton->setPosition(worldPos);

        auto jumpButtonMenu = CCMenu::createWithItem(jumpButton);
        jumpButtonMenu->setID(JumpButton::id + "-menu");
        jumpButtonMenu->setPosition({0, 0});

        m_secondaryLayer->addChild(jumpButtonMenu);
    }

    void onJumpButton(CCObject* sender)
    {
        auto gsm = GameStatsManager::sharedState();

        const auto pageSize = 4 * 3;
        auto pageLayers = CCArrayExt<CCLayer*>(m_secondaryScrollLayer->m_extendedLayer->getChildren());

        jti::scroll::toIncompletePage(m_secondaryScrollLayer, pageSize, m_fields->getChestCount(),
            [&pageLayers](int pageIndex, int itemIndex) {
                //not gonna decipher what SecretRewardsLayer::generateChestItems does, so I'm using nodes
                auto chestButtons = CCArrayExt<CCMenuItemSpriteExtra*>(pageLayers[pageIndex]->getChildByType<CCMenu*>(0)->getChildren());
                auto chestId = chestButtons[itemIndex % pageSize]->getTag();
                return GameStatsManager::sharedState()->isSecretChestUnlocked(chestId);
            },
            [](BoomScrollLayer* bsl, int pagesForward) {
                if (bsl->m_looped)
                    jti::scroll::instantJumpForward(bsl, pagesForward);
                else //25-key chests; emulate wrap-around
                    bsl->instantMoveToPage((bsl->m_page + pagesForward) % bsl->getTotalPages()); //0 -> 1 or 0, 1 -> 0 or 1 (overengineered edition)
            },
            (CCNode*)sender);
    }
};