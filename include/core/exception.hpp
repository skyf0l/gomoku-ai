/**
 * @file exceptions.hpp
 * @brief Exception classes for the protocol
 */

#ifndef CORE_EXCEPTION_HPP
#define CORE_EXCEPTION_HPP

#include <string>

namespace gmk {

class KeyNotFoundException : public std::exception {
public:
    KeyNotFoundException(const std::string &key)
        : m_key(key)
    {
        m_what = "Key not found : " + key;
    }

    const char *what() const throw() override
    {
        return m_what.c_str();
    }

private:
    std::string m_what;

    std::string m_key;
};

class InvalidArgumentCountException : public std::exception {
public:
    InvalidArgumentCountException(const std::string &verb, std::uint8_t expected, std::uint8_t given)
        : m_verb(verb)
        , m_expected(expected)
        , m_given(given)
    {
        m_what = "Invalid argument count for verb " + m_verb + " : expected " + std::to_string(m_expected) + " given "
            + std::to_string(m_given);
    }

    const char *what() const throw() override
    {
        return m_what.c_str();
    }

private:
    std::string m_what;

    std::string m_verb;
    std::uint8_t m_expected;
    std::uint8_t m_given;
};

class WrongArgumentTypeException : public std::exception {
public:
    WrongArgumentTypeException(const std::string &verb, const std::string &argument, const std::string &expected)
        : m_verb(verb)
        , m_argument(argument)
        , m_expected(expected)
    {
        m_what = "Wrong argument type for verb " + m_verb + " : " + m_argument + " is not " + m_expected;
    }

    const char *what() const throw() override
    {
        return m_what.c_str();
    }

private:
    std::string m_what;

    std::string m_verb;
    std::string m_argument;
    std::string m_expected;
};

}

#endif // CORE_EXCEPTION_HPP