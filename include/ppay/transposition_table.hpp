/**
 * @file transposition_table.hpp
 * @brief Transposition table Position -> Score
 */

#ifndef PPAY_TRANSPOSITION_TABLE_HPP
#define PPAY_TRANSPOSITION_TABLE_HPP

#include <algorithm>
#include <unordered_map>

namespace gmk::ppay {

template <typename Key, typename Value>
class TranspositionTable {
public:
    TranspositionTable(std::size_t maxSize)
        : m_maxSize(maxSize)
    {
    }

    inline bool get(const Key &key, int &value)
    {
        auto it = m_table.find(key);
        if (it != m_table.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

    inline void set(const Key &key, Value value)
    {
        if (m_table.size() >= m_maxSize)
            m_table.erase(m_table.begin());
        m_table[key] = value;
    }

    inline void resize(std::size_t maxSize)
    {
        if (maxSize < m_maxSize) {
            m_table.erase(m_table.begin(), std::next(m_table.begin(), m_table.size() - maxSize));
        }
        m_maxSize = maxSize;
    }

    inline void clear()
    {
        m_table.clear();
    }

    inline std::size_t size() const
    {
        return m_table.size();
    }

private:
    std::size_t m_maxSize;
    std::unordered_map<Key, Value> m_table;
};

}

#endif /* PPAY_TRANSPOSITION_TABLE_HPP */