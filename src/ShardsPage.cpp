#include <Geode/Geode.hpp>
#include <Geode/modify/ShardsPage.hpp>
#include "JumpButton.h"

using namespace geode::prelude;

enum class ShardStat
{
    Fire = 18,
    Ice = 19,
    Poison = 17,
    Shadow = 16,
    Lava = 20,
    Earth = 23,
    Blood = 24,
    Metal = 25,
    Light = 26,
    Soul = 27
};

std::array g_shardTypes = {ShardStat::Fire, ShardStat::Ice, ShardStat::Poison, ShardStat::Shadow, ShardStat::Lava, ShardStat::Earth, ShardStat::Blood, ShardStat::Metal, ShardStat::Light, ShardStat::Soul};

class $modify(MyShardsPage, ShardsPage)
{
    struct Fields
    {
        std::array<int, g_shardTypes.size()> shards;
    };

    $override bool init()
    {
        if (!ShardsPage::init())
            return false;
        
        auto gsm = GameStatsManager::sharedState();
        for (int i = 0; i < g_shardTypes.size(); i++)
            m_fields->shards[i] = gsm->getStat(fmt::to_string((int)g_shardTypes[i]).c_str());

        for (int page = m_page + 1; page < m_pages->count(); page++)
        {
            //TODO replace with two ifs using page%2
            switch (page)
            {
                case 1:
                    if (*std::ranges::min_element(m_fields->shards.begin(), m_fields->shards.begin() + 5) < 100)
                        goto jump;
                    break;
                case 2:
                    if (std::min({m_fields->shards[5], m_fields->shards[6], m_fields->shards[7]}) < 100)
                        goto jump;
                    break;
                case 3:
                    if (*std::ranges::min_element(m_fields->shards.begin() + 5, m_fields->shards.begin() + 10) < 100)
                        goto jump;
                    break;
                default:
                    log::warn("unhandled ShardsPage page {}", page);
            }

            continue;

        jump:
            for (int i = m_page; i < page; i++)
                onSwitchPage(m_nextButton);
            break;
        }

        return true;
    }
};