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

std::array g_shardStats = {ShardStat::Fire, ShardStat::Ice, ShardStat::Poison, ShardStat::Shadow, ShardStat::Lava, ShardStat::Earth, ShardStat::Blood, ShardStat::Metal, ShardStat::Light, ShardStat::Soul};

//not adding a button since it's just a few fully visible pages
class $modify(ShardsPage)
{
    struct Fields
    {
        std::array<int, g_shardStats.size()> shards;
    };

    $override bool init()
    {
        if (!ShardsPage::init())
            return false;

        auto gsm = GameStatsManager::sharedState();
        const auto maxShards = 100;

        for (int i = 0; i < g_shardStats.size(); i++)
            m_fields->shards[i] = gsm->getStat(fmt::to_string((int)g_shardStats[i]).c_str());

        for (int page = 0; page < m_pages->count(); page++)
        {
            auto groupOffset = page / 2 * 5;

            if (page % 2 == 0)
            {
                auto& shards = m_fields->shards;
                if (std::min({shards[groupOffset], shards[groupOffset + 1], shards[groupOffset + 2]}) < maxShards)
                    goto jump;
            }
            else
            {
                //check the bonus reward
                auto rangeStart = m_fields->shards.begin() + groupOffset;
                if (*std::ranges::min_element(rangeStart, rangeStart + 5) < maxShards)
                    goto jump;
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