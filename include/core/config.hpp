/**
 * @file config.hpp
 * @brief Game configuration
 */

#ifndef CORE_CONFIG_HPP
#define CORE_CONFIG_HPP

#include <string>

namespace gmk {

struct Config {
    std::uint32_t board_width;
    std::uint32_t board_height;
    std::uint32_t timeout_turn;
    std::uint32_t timeout_match;
    std::uint32_t max_memory;
    std::uint32_t time_left;
    enum class GameType {
        human_opponent,
        ai_opponent,
        tournament,
        network_tournament,
    } game_type;
    struct {
        bool exactly_five;
        bool continuous_game;
        bool renju;
    } rule;
    // The evaluate info is ommited as it is perfectly useless for u
    std::string persistent_folder;
};

}

#endif /* CORE_CONFIG_HPP */