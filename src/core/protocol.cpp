#include "core/protocol.hpp"

namespace gmk {

// This looks kind of ugly, though it is so fast that it doesn't matter
std::unordered_map<std::string, IncomingVerb> const string_incoming_verb_map = {
    { "START", IncomingVerb::start },
    { "TURN", IncomingVerb::turn },
    { "BEGIN", IncomingVerb::begin },
    { "BOARD", IncomingVerb::board },
    { "INFO timeout_turn", IncomingVerb::info_timeout_turn },
    { "INFO timeout_match", IncomingVerb::info_timeout_match },
    { "INFO max_memory", IncomingVerb::info_max_memory },
    { "INFO time_left", IncomingVerb::info_time_left },
    { "INFO game_type", IncomingVerb::info_game_type },
    { "INFO rule", IncomingVerb::info_rule },
    { "INFO folder", IncomingVerb::info_folder },
    { "INFO evaluate", IncomingVerb::info_evaluate },
    { "END", IncomingVerb::end },
    { "ABOUT", IncomingVerb::about },
    { "RECTSTART", IncomingVerb::rect_start },
    { "RESTART", IncomingVerb::restart },
    { "TAKEBACK", IncomingVerb::takeback },
    { "PLAY", IncomingVerb::play },
};

std::unordered_map<OutgoingVerb, std::string> const outgoing_verb_string_map = {
    { OutgoingVerb::unknown, "UNKNOWN" },
    { OutgoingVerb::error, "ERROR" },
    { OutgoingVerb::message, "MESSAGE" },
    { OutgoingVerb::debug, "DEBUG" },
    { OutgoingVerb::suggest, "SUGGEST" },
};

std::unordered_map<IncomingVerb, VerbConfig> const incoming_verb_config_map = {
    { IncomingVerb::start,
        {
            .numPerLine = 1,
            .type = PossibleTypes::integer,
            .multiLine = false,
        } },
    { IncomingVerb::turn,
        {
            .numPerLine = 2,
            .type = PossibleTypes::integer,
            .multiLine = false,
            .separator = ",",
        } },
    { IncomingVerb::begin,
        {
            .numPerLine = 0,
            .type = PossibleTypes::none,
            .multiLine = false,
        } },
    { IncomingVerb::board,
        {
            .numPerLine = 3,
            .type = PossibleTypes::integer,
            .multiLine = true,
            .endMultiLine = "DONE",
            .separator = ",",
        } },
    { IncomingVerb::info_timeout_turn,
        {
            .numPerLine = 1,
            .type = PossibleTypes::integer,
            .multiLine = false,
        } },
    { IncomingVerb::info_timeout_match,
        {
            .numPerLine = 1,
            .type = PossibleTypes::integer,
            .multiLine = false,
        } },
    { IncomingVerb::info_max_memory,
        {
            .numPerLine = 1,
            .type = PossibleTypes::integer,
            .multiLine = false,
        } },
    { IncomingVerb::info_time_left,
        {
            .numPerLine = 1,
            .type = PossibleTypes::integer,
            .multiLine = false,
        } },
    { IncomingVerb::info_game_type,
        {
            .numPerLine = 1,
            .type = PossibleTypes::integer,
            .multiLine = false,
        } },
    { IncomingVerb::info_rule,
        {
            .numPerLine = 1,
            .type = PossibleTypes::integer,
            .multiLine = false,
        } },
    { IncomingVerb::info_evaluate,
        {
            .numPerLine = 2,
            .type = PossibleTypes::integer,
            .multiLine = false,
            .separator = ",",
        } },
    { IncomingVerb::info_folder,
        {
            .numPerLine = 1,
            .type = PossibleTypes::string,
            .multiLine = false,
        } },
    { IncomingVerb::end,
        {
            .numPerLine = 0,
            .type = PossibleTypes::none,
            .multiLine = false,
        } },
    { IncomingVerb::about,
        {
            .numPerLine = 0,
            .type = PossibleTypes::none,
            .multiLine = false,
        } },
    { IncomingVerb::rect_start,
        {
            .numPerLine = 2,
            .type = PossibleTypes::integer,
            .multiLine = false,
            .separator = ",",
        } },
    { IncomingVerb::restart,
        {
            .numPerLine = 0,
            .type = PossibleTypes::none,
            .multiLine = false,
        } },
    { IncomingVerb::takeback,
        {
            .numPerLine = 2,
            .type = PossibleTypes::integer,
            .multiLine = false,
            .separator = ",",
        } },
    { IncomingVerb::play,
        {
            .numPerLine = 2,
            .type = PossibleTypes::integer,
            .multiLine = false,
            .separator = ",",
        } },
};

}
