/**
 * @file brain_core.hpp
 * @brief Core of the brain (protocol, etc.)
 */

#ifndef CORE_BRAIN_CORE_HPP
#define CORE_BRAIN_CORE_HPP

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#ifdef DEBUG
#include <fstream>
#endif

#include "core/config.hpp"
#include "core/protocol.hpp"

namespace gmk {

using Arguments = std::vector<std::variant<std::int32_t, std::string>>;

class BrainCore {
public:
    enum InfoType {
        timeout_turn,
        timeout_match,
        max_memory,
        time_left,
        game_type,
        rule,
        folder,
        evaluate,
    };

    BrainCore(const std::string &about = "");
    virtual ~BrainCore();

    //! NEED TO BE IMPLEMENTED BY THE BRAIN

    // create the board and call sendOK() or sendError("msg"), return false if error
    virtual bool brainInit() = 0;

    // delete old board, create new board, call sendOK(), return false if error
    virtual bool brainRestart() = 0;

    // callback for info update
    virtual void brainInfo(const InfoType &info) = 0;

    // choose your move and call doMyMove(x,y), x < width, y < height
    virtual void brainTurn() = 0;

    // put your move to the boardn, return true if success
    virtual bool brainMyMove(std::uint32_t x, std::uint32_t y) = 0;

    // put opponent's move to the board, return true if success
    virtual bool brainOpponentMove(std::uint32_t x, std::uint32_t y) = 0;

    // square [x,y] belongs to a winning line (when info_continuous is 1), return true if success
    virtual bool brainBlock(std::uint32_t x, std::uint32_t y) = 0;

    // clear one square, return value: success, not supported, error
    virtual void brainTakeback(std::uint32_t x, std::uint32_t y) = 0;

    // delete temporary files, free resources
    virtual void brainEnd() = 0;

    //! BUILT-IN FUNCTIONS

    // core
    void run();
    void executeCommand(const std::string &command);

    Arguments parse_arguments(std::string const &verb, VerbConfig const &verb_config, std::string const &str_args);
    Arguments parse_multiline_arguments(std::string const &verb, VerbConfig const &verb_config);
    std::tuple<IncomingVerb, Arguments> parse_command(std::string const &command_);

    void startThinkingThread();
    void stopThinkingThread();
    void thinkingThread();
    void startThinking();

    // callbacks
    void doMyMove(std::uint32_t x, std::uint32_t y);

    // communication
    bool recv(std::string &buffer);
    void send(const std::string &str);
    void sendOK();
    void sendUnknown();
    void sendUnknown(const std::string &str);
    void sendError();
    void sendError(const std::string &str);
    void sendMessage(const std::string &str);
    void sendDebug(const std::string &str);
    void sendSuggest(std::uint32_t x, std::uint32_t y);

protected:
    // core
    bool m_running;
    const std::string m_about;

    bool m_thinking_thread_running;
    std::thread m_thinking_thread;
    std::condition_variable m_need_thinking_cond;
    std::condition_variable m_start_thinking_cond;

    Config m_config;

#ifdef DEBUG
    std::ofstream m_debug_file;
#endif
};

}

#endif /* CORE_BRAIN_CORE_HPP */