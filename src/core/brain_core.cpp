#include <exception>
#include <iostream>
#include <sstream>

#include "core/brain_core.hpp"
#include "core/exception.hpp"

namespace gmk {

BrainCore::BrainCore(const std::string &about)
    : m_about(about)
{
    m_config = {
        .board_width = 20,
        .board_height = 20,
        .timeout_turn = 5 * 1000, // in ms
        .timeout_match = 180 * 1000, // in ms
        .max_memory = 70 * 1024 * 1024, // in KB
        .time_left = 180 * 1000, // in ms
        .game_type = Config::GameType::human_opponent,
        .rule = {
            .exactly_five = false,
            .continuous_game = false,
            .renju = false,
        },
        .persistent_folder = "",
    };
}

BrainCore::~BrainCore()
{
#ifdef DEBUG
    // close the debug file
    if (m_debug_file.is_open()) {
        m_debug_file << "Stopping brain" << std::endl;
        m_debug_file.close();
    }
#endif
}

void BrainCore::run()
{
    sendMessage("Gomoku AI should not be started directly. "
                "Please install gomoku manager (http://sourceforge.net/projects/piskvork). "
                "Then enter path to this exe file in players settings.");

#ifdef DEBUG
    // open the debug file
    m_debug_file = std::ofstream(DEBUG_DIR "/debug.log", std::ios::app);
    if (!m_debug_file.is_open()) {
        sendError("Cannot open debug file: " DEBUG_DIR "/debug.log");
        return;
    }
    m_debug_file << "Starting brain" << std::endl;
#endif

    std::string line;
    m_running = true;
    m_thinking_thread_running = false;
    while (m_running) {
        if (recv(line)) {
            if (line.empty()) {
                continue;
            }
            executeCommand(line);
        } else {
            stopThinkingThread();
            brainEnd();
            m_running = false;
        }
    }
}

void BrainCore::startThinkingThread()
{
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);

    // start if not running
    if (!m_thinking_thread.joinable()) {
        m_thinking_thread_running = true;
        m_thinking_thread = std::thread(&BrainCore::thinkingThread, this);

        // wait for the thread to start
        m_start_thinking_cond.wait(lock);
    }
}

void BrainCore::stopThinkingThread()
{
    // stop if running
    m_thinking_thread_running = false;
    if (m_thinking_thread.joinable()) {
        m_need_thinking_cond.notify_one();
        m_thinking_thread.join();
    }
}

void BrainCore::thinkingThread()
{
    std::mutex thinking_mutex;
    std::unique_lock<std::mutex> lock(thinking_mutex);

    // confirm that we are running
    m_start_thinking_cond.notify_one();
    // wait for the first move
    m_need_thinking_cond.wait(lock);
    while (m_running && m_thinking_thread_running) {
        brainTurn();
        // wait for the next move
        m_need_thinking_cond.wait(lock);
    }
}

void BrainCore::startThinking()
{
    // unlock thinking thread
    m_need_thinking_cond.notify_one();
}

#define NEED_THINKING_THREAD_RUNNING                                                                                                                           \
    if (!m_thinking_thread_running) {                                                                                                                          \
        sendError("Thinking thread is not running");                                                                                                           \
        break;                                                                                                                                                 \
    }

void BrainCore::executeCommand(const std::string &command)
{
    try {
        std::tuple<IncomingVerb, Arguments> c = parse_command(command);
        IncomingVerb verb = std::get<0>(c);
        Arguments args = std::get<1>(c);

        switch (verb) {
        // start and stop
        case IncomingVerb::start:
            if (std::get<std::int32_t>(args[0]) < 0) {
                sendError("Board size must be positive");
                stopThinkingThread();
                break;
            }
            m_config.board_width = m_config.board_height = std::get<std::int32_t>(args[0]);
            if (brainInit())
                startThinkingThread();
            else
                stopThinkingThread();
            break;
        case IncomingVerb::rect_start:
            if (std::get<std::int32_t>(args[0]) < 0 || std::get<std::int32_t>(args[1]) < 0) {
                sendError("Board size must be positive");
                stopThinkingThread();
                break;
            }
            m_config.board_width = std::get<std::int32_t>(args[0]);
            m_config.board_height = std::get<std::int32_t>(args[1]);
            if (brainInit())
                startThinkingThread();
            else
                stopThinkingThread();
            break;
        case IncomingVerb::restart:
            NEED_THINKING_THREAD_RUNNING;
            if (brainRestart())
                startThinkingThread();
            else
                stopThinkingThread();
            break;
        case IncomingVerb::end:
            m_running = false;
            stopThinkingThread();
            brainEnd();
            break;

        // moves
        case IncomingVerb::turn:
            if (std::get<std::int32_t>(args[0]) < 0 || std::get<std::int32_t>(args[1]) < 0) {
                sendError("Move values must be positive");
                break;
            }
            NEED_THINKING_THREAD_RUNNING;
            if (brainOpponentMove(std::get<std::int32_t>(args[0]), std::get<std::int32_t>(args[1])))
                startThinking();
            break;
        case IncomingVerb::begin:
            NEED_THINKING_THREAD_RUNNING;
            startThinking();
            break;
        case IncomingVerb::takeback:
            if (std::get<std::int32_t>(args[0]) < 0 || std::get<std::int32_t>(args[1]) < 0) {
                sendError("Takeback values must be positive");
                break;
            }
            NEED_THINKING_THREAD_RUNNING;
            brainTakeback(std::get<std::int32_t>(args[0]), std::get<std::int32_t>(args[1]));
            break;
        case IncomingVerb::play:
            if (std::get<std::int32_t>(args[0]) < 0 || std::get<std::int32_t>(args[1]) < 0) {
                sendError("Move values must be positive");
                break;
            }
            NEED_THINKING_THREAD_RUNNING;
            brainMyMove(std::get<std::int32_t>(args[0]), std::get<std::int32_t>(args[1]));
            break;
        case IncomingVerb::board: {
            NEED_THINKING_THREAD_RUNNING;

            bool hasError = false;
            // for 3 by 3 arguments
            for (std::uint32_t i = 0; i < args.size(); i += 3) {
                if (std::get<std::int32_t>(args[0]) < 0 || std::get<std::int32_t>(args[1]) < 0) {
                    sendError("Move values must be positive");
                    hasError = true;
                    break;
                }
                int x = std::get<std::int32_t>(args[i]);
                int y = std::get<std::int32_t>(args[i + 1]);
                int type = std::get<std::int32_t>(args[i + 2]);
                switch (type) {
                case 1:
                    if (!brainMyMove(x, y))
                        hasError = true;
                    break;
                case 2:
                    if (!brainOpponentMove(x, y))
                        hasError = true;
                    break;
                case 3:
                    if (!brainBlock(x, y))
                        hasError = true;
                    break;
                default:
                    sendError("Unknown board state");
                }
                if (hasError)
                    break;
            }
            if (!hasError)
                startThinking();
        } break;

        // info and config
        case IncomingVerb::info_timeout_turn:
            m_config.timeout_turn = static_cast<uint32_t>(std::max(0, std::get<std::int32_t>(args[0])));
            brainInfo(InfoType::timeout_turn);
            break;
        case IncomingVerb::info_timeout_match:
            m_config.timeout_match = static_cast<uint32_t>(std::max(0, std::get<std::int32_t>(args[0])));
            brainInfo(InfoType::timeout_match);
            break;
        case IncomingVerb::info_max_memory:
            m_config.max_memory = static_cast<uint32_t>(std::max(0, std::get<std::int32_t>(args[0])));
            brainInfo(InfoType::max_memory);
            break;
        case IncomingVerb::info_time_left:
            m_config.time_left = static_cast<uint32_t>(std::max(0, std::get<std::int32_t>(args[0])));
            brainInfo(InfoType::time_left);
            break;
        case IncomingVerb::info_game_type: {
            std::uint32_t type = static_cast<uint32_t>(std::max(0, std::get<std::int32_t>(args[0])));
            if (type == 0) {
                m_config.game_type = Config::GameType::human_opponent;
            } else if (type == 1) {
                m_config.game_type = Config::GameType::ai_opponent;
            } else if (type == 2) {
                m_config.game_type = Config::GameType::tournament;
            } else if (type == 3) {
                m_config.game_type = Config::GameType::network_tournament;
            } else {
                sendError("Unknown game type");
                break;
            }
            brainInfo(InfoType::game_type);
        } break;
        case IncomingVerb::info_rule: {
            std::uint32_t rule = static_cast<uint32_t>(std::max(0, std::get<std::int32_t>(args[0])));
            m_config.rule.exactly_five = (rule & 1) != 0;
            m_config.rule.continuous_game = ((rule >> 1) & 1) != 0;
            m_config.rule.renju = ((rule >> 2) & 1) != 0;
            brainInfo(InfoType::rule);
        } break;
        case IncomingVerb::info_folder:
            m_config.persistent_folder = std::get<std::string>(args[0]);
            brainInfo(InfoType::folder);
            break;

        // other
        case IncomingVerb::about:
            send(m_about);
            break;
        default:
            sendUnknown();
        }
    } catch (const KeyNotFoundException &e) {
        sendUnknown(e.what());
        return;
    } catch (const std::exception &e) {
        sendError(e.what());
        return;
    }
}

void BrainCore::doMyMove(std::uint32_t x, std::uint32_t y)
{
    brainMyMove(x, y);
    send(std::to_string(x) + "," + std::to_string(y));
}

static std::string trim(std::string const &str)
{
    std::string::size_type const first = str.find_first_not_of(" \t\n\r");
    std::string::size_type const last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

Arguments BrainCore::parse_arguments(std::string const &verb, VerbConfig const &verb_config, std::string const &str_args)
{
    Arguments args;
    if (verb_config.numPerLine == 0)
        return args;

    std::stringstream ss(trim(str_args));
    std::string str_arg;

    for (int i = 0; i < verb_config.numPerLine; i++) {
        // for each argument

        if (std::getline(ss, str_arg, verb_config.separator[0])) {
            str_arg = trim(str_arg);
            if (verb_config.type == PossibleTypes::integer) {
                // if integer expected
                try {
                    int value = std::stoi(str_arg);
                    args.push_back(static_cast<int32_t>(value));
                } catch (std::invalid_argument const &e) {
                    throw WrongArgumentTypeException(verb, str_arg, "int32_t");
                }
            } else if (verb_config.type == PossibleTypes::string) {
                // if string expected
                args.push_back(str_arg);
            }
        } else {
            throw InvalidArgumentCountException(verb, i, verb_config.numPerLine);
        }
    }
    if (std::getline(ss, str_arg)) {
        throw InvalidArgumentCountException(verb, verb_config.numPerLine, verb_config.numPerLine);
    }

    return args;
}

Arguments BrainCore::parse_multiline_arguments(std::string const &verb, VerbConfig const &verb_config)
{
    Arguments args;

    std::string str_arg;
    while (recv(str_arg)) {
        str_arg = trim(str_arg);
        if (str_arg == verb_config.endMultiLine)
            break;
        Arguments next_args = parse_arguments(verb, verb_config, str_arg);
        args.insert(args.end(), next_args.begin(), next_args.end());
    }

    return args;
}

std::tuple<IncomingVerb, Arguments> BrainCore::parse_command(std::string const &command_)
{
    std::string command = trim(command_);
    for (auto const &pair : string_incoming_verb_map) {
        // for each verb
        if (command.rfind(pair.first, 0) == 0) {
            // if verb found
            IncomingVerb verb = pair.second;
            VerbConfig const &verb_config = incoming_verb_config_map.at(verb);
            if (command.length() == pair.first.length()) {
                // if no arguments
                if (verb_config.multiLine) {
                    // if multiline
                    return std::make_tuple(verb, parse_multiline_arguments(pair.first, verb_config));
                } else {
                    // if not multiline
                    if (verb_config.numPerLine == 0) {
                        return std::make_tuple(verb, Arguments());
                    } else {
                        throw InvalidArgumentCountException(pair.first, 0, verb_config.numPerLine);
                    }
                }
            } else if (command[pair.first.length()] == ' ') {
                // if arguments
                if (verb_config.multiLine) {
                    // can't be multiline with arguments (yet)
                    throw InvalidArgumentCountException(pair.first, -1, 0);
                } else {
                    // if not multiline
                    return std::make_tuple(verb, parse_arguments(pair.first, verb_config, command.substr(pair.first.length() + 1)));
                }
            }
        }
    }
    throw KeyNotFoundException(command);
}

bool BrainCore::recv(std::string &buffer)
{
    if (getline(std::cin, buffer)) {
#ifdef DEBUG
        m_debug_file << "<< " << buffer << std::endl;
#endif
        return true;
    }
    return false;
}

void BrainCore::send(const std::string &str)
{
    std::cout << str << std::endl;
#ifdef DEBUG
    m_debug_file << ">> " << str << std::endl;
#endif
}

void BrainCore::sendOK()
{
    send("OK");
}

void BrainCore::sendUnknown()
{
    send("UNKNOWN");
}

void BrainCore::sendUnknown(const std::string &str)
{
    send("UNKNOWN " + str);
}

void BrainCore::sendError()
{
    send("ERROR");
}

void BrainCore::sendError(const std::string &str)
{
    send("ERROR " + str);
}

void BrainCore::sendMessage(const std::string &str)
{
    send("MESSAGE " + str);
}

void BrainCore::sendDebug(const std::string &str)
{
    send("DEBUG " + str);
}

void BrainCore::sendSuggest(std::uint32_t x, std::uint32_t y)
{
    send("SUGGEST " + std::to_string(x) + " " + std::to_string(y));
}
}
