#include <Geode/Geode.hpp>
#include <Geode/modify/SecretRewardsLayer.hpp>
#include "JumpButton.h"
#include "utils.h"

using namespace geode::prelude;

class $modify(JtiSecretRewardsLayer, SecretRewardsLayer)
{
    struct Fields
    {
        GJRewardType chestType;
    };

    $override void createSecondaryLayer(int chestType)
    {
        SecretRewardsLayer::createSecondaryLayer(chestType);

        if (m_secondaryScrollLayer->m_extendedLayer->getChildren()->count() == 1)
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
        //not gonna decipher what SecretRewardsLayer::generateChestItems does
        auto chestsLayer = m_secondaryScrollLayer->m_extendedLayer;
        auto pageLayers = CCArrayExt<CCLayer*>(chestsLayer->getChildren());

        auto pageCount = m_secondaryScrollLayer->getTotalPages();
        auto startingPage = jti::utils::floorMod(m_secondaryScrollLayer->m_page, pageCount);
        log::debug("pageCount {}", pageCount);

        const auto pageSize = 4 * 3;
        for (int i = 0; i < pageCount; i++) //check the next pageCount pages
        {
            auto pageIndex = (startingPage + 1/*start at next page*/ + i) % pageCount;
            auto chestButtons = CCArrayExt<CCMenuItemSpriteExtra*>(pageLayers[pageIndex]->getChildByType<CCMenu*>(0)->getChildren());
            auto pageItemCount = chestButtons.size();
            log::debug("checking page {} with {} items", pageIndex, pageItemCount);

            for (int itemIndex = 0; itemIndex < pageItemCount; itemIndex++) //check all items on the page
            {
                auto id = chestButtons[itemIndex]->getTag();
                if (!GameStatsManager::sharedState()->isSecretChestUnlocked(id))
                {
                    if (m_fields->chestType < GJRewardType::Key25Treasure) //25, 50 and 100 key chests don't have wrap-around
                    {
                        //(instant)moveToPage sometimes results in empty pages
                        log::debug("jumping {} pages forward", i + 1);
                        for (int k = 0; k < i + 1; k++)
                            onSwitchPage(m_rightButton);
                    }
                    else
                        m_secondaryScrollLayer->moveToPage(pageIndex);

                    return;
                }
            }
        }

        log::debug("no item found");
        ((CCNode*)sender)->setVisible(false);
    }
};