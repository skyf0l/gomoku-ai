/**
 * @file protocol.hpp
 * @brief Implements the protocol for the communication between the brain and the manager
 *
 * This whole file is kind of a mess, but it should work
 * To read about the protocol documentation you can go there :
 * https://svn.code.sf.net/p/piskvork/code/trunk/source/doc/protocl2en.htm
 * (Good link as of 2022-01-29, if it's not then go to the internet archive,
 * it should be archived)
 */

#ifndef CORE_PROTOCOL_HPP
#define CORE_PROTOCOL_HPP

#include <string>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

namespace gmk {

enum class OutgoingVerb {
    unknown,
    error,
    message,
    debug,
    suggest,
};

enum class IncomingVerb {
    start,
    turn,
    begin,
    board,
    info_timeout_turn,
    info_timeout_match,
    info_max_memory,
    info_time_left,
    info_game_type,
    info_rule,
    info_evaluate,
    info_folder,
    end,
    about,
    rect_start,
    restart,
    takeback,
    play,
};

enum class PossibleTypes {
    integer,
    string,
    none,
};

struct VerbConfig {
    std::uint8_t numPerLine;
    PossibleTypes type;
    bool multiLine;
    std::string endMultiLine;
    std::string separator;
};

extern std::unordered_map<std::string, IncomingVerb> const string_incoming_verb_map;
extern std::unordered_map<OutgoingVerb, std::string> const outgoing_verb_string_map;
extern std::unordered_map<IncomingVerb, VerbConfig> const incoming_verb_config_map;

}

#endif /* CORE_PROTOCOL_HPP */
