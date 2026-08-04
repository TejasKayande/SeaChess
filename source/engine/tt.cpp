#include "tt.hpp"
#include <iostream>

namespace Engine {

    TranspositionTable::TranspositionTable(size_t sizeInMB) {
        resize(sizeInMB);
    }

    void TranspositionTable::resize(size_t sizeInMB) {
        size_t numEntries = (sizeInMB * 1024 * 1024) / sizeof(TTEntry);
        m_size = numEntries;
        m_table.resize(m_size);
        clear();
    }

    void TranspositionTable::clear() {
        for (auto& entry : m_table) {
            entry.key = 0;
            entry.flag = TTFlag::NONE;
        }
    }

    void TranspositionTable::store(u64 key, int depth, int score, TTFlag flag, Move bestMove) {
        if (m_size == 0) return;

        size_t index = key % m_size;
        TTEntry& entry = m_table[index];

        // Always replace if depth is greater or equal, or if it's a completely different position (or empty)
        // A more advanced replacement scheme could be used later
        if (entry.key != key || depth >= entry.depth || entry.flag == TTFlag::NONE) {
            entry.key = key;
            entry.depth = depth;
            entry.score = score;
            entry.flag = flag;
            entry.bestMove = bestMove;
        }
    }

    bool TranspositionTable::probe(u64 key, int depth, int alpha, int beta, int& returnScore, Move& bestMove) {
        if (m_size == 0) return false;

        size_t index = key % m_size;
        TTEntry& entry = m_table[index];

        if (entry.key == key) {
            bestMove = entry.bestMove;

            if (entry.depth >= depth) {
                if (entry.flag == TTFlag::EXACT) {
                    returnScore = entry.score;
                    return true;
                }
                if (entry.flag == TTFlag::UPPERBOUND && entry.score <= alpha) {
                    returnScore = entry.score; // alpha
                    return true;
                }
                if (entry.flag == TTFlag::LOWERBOUND && entry.score >= beta) {
                    returnScore = entry.score; // beta
                    return true;
                }
            }
        }

        return false;
    }

} // namespace Engine
