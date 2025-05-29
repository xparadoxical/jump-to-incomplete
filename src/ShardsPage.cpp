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

//not adding a button since it's just a few fully visible pages
class $modify(ShardsPage)
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
        const auto maxShards = 100;

        for (int i = 0; i < g_shardTypes.size(); i++)
            m_fields->shards[i] = gsm->getStat(fmt::to_string((int)g_shardTypes[i]).c_str());

        for (int page = m_page + 1; page < m_pages->count(); page++)
        {
            auto groupOffset = page / 2 * 5;

            if (page % 2 == 0)
            {
                if (std::min({m_fields->shards[groupOffset], m_fields->shards[groupOffset + 1], m_fields->shards[groupOffset + 2]}) < maxShards)
                    goto jump;
            }
            else
            {
                //check the bonus reward
                if (*std::ranges::min_element(m_fields->shards.begin() + groupOffset, m_fields->shards.begin() + groupOffset + 5) < maxShards)
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