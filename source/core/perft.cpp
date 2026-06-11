
#include "perft.hpp"
#include <bitset>

unsigned long PerfTest::perft(Chess::Board* board, int depth) {

    if (depth == 0) return 1UL;

    MoveList move_list;
    move_list.clear();

    MoveGen::Legal::generateAllMoves(board, move_list);

    unsigned long nodes = 0;

    for (const Move& move : move_list) {

        Chess::Board copy = *board;
        if (copy.makeMove(move)) nodes += perft(&copy, depth - 1);
    }

    return nodes;
}

void PerfTest::divide(Chess::Board* board, int depth) {

    using namespace std::chrono;

    MoveList move_list;
    move_list.clear();

    MoveGen::Legal::generateAllMoves(board, move_list);

    unsigned long total = 0;

    auto start = high_resolution_clock::now();

    MoveList sorted_moves;
    for (int i = 0; i < move_list.size(); i++) sorted_moves.push_back(move_list[i]);
    std::sort(sorted_moves.begin(), sorted_moves.end(), [](const Move& a, const Move& b) {
        std::string sa =
            a.from.toString() + a.to.toString();

        std::string sb =
            b.from.toString() + b.to.toString();

        return sa < sb;
    });

    for (const Move& move : sorted_moves) {

        Chess::Board copy = *board;

        if (copy.makeMove(move)) {
            unsigned long nodes = perft(&copy, depth - 1);
            total += nodes;
            std::cout << move.from.toString() << move.to.toString() << ": " << nodes << "\n";
        }
    }

    auto end = high_resolution_clock::now();

    auto ms = duration_cast<milliseconds>(end - start).count();

    std::cout << "\nDepth : " << depth << "\n";
    std::cout << "Nodes : "   << total << "\n";
    std::cout << "Time  : "   << ms    << " ms\n";

    if (ms > 0) std::cout << "NPS   : " << (total * 1000ULL) / ms << "\n";
}

void PerfTest::runPerftest(Chess::Board *board, int depth) {

    MoveGen::init();
    divide(board, depth);
}