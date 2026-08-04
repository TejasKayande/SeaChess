#pragma once

#include "../core/base.hpp"
#include "../core/move.hpp"
#include <vector>

namespace Engine {

    enum class TTFlag {
        EXACT,
        LOWERBOUND,
        UPPERBOUND,
        NONE
    };

    struct TTEntry {
        u64 key = 0;
        Move bestMove;
        int score = 0;
        int depth = 0;
        TTFlag flag = TTFlag::NONE;
    };

    class TranspositionTable {
    private:
        std::vector<TTEntry> m_table;
        size_t m_size;

    public:
        TranspositionTable(size_t sizeInMB);
        void resize(size_t sizeInMB);
        void clear();

        void store(u64 key, int depth, int score, TTFlag flag, Move bestMove);
        bool probe(u64 key, int depth, int alpha, int beta, int& returnScore, Move& bestMove);
    };

} // namespace Engine
