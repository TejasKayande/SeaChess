
#include "perft.hpp"

unsigned long PerfTest::perft(Chess::Board* board, int depth) {

    if (depth == 0) return 1UL;

    MoveList move_list;
    move_list.clear();

    MoveGen::Legal::generateAllMoves(board, move_list);

    unsigned long nodes = 0;

    for (const Move& move : move_list) {

        Chess::Board copy = *board;

        if (copy.makeMove(move)) {

            copy.changeTurn();

            nodes += perft(&copy, depth - 1);
        }
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

    for (const Move& move : move_list) {

        Chess::Board copy = *board;

        if (copy.makeMove(move)) {

            copy.changeTurn();

            unsigned long nodes = perft(&copy, depth - 1);

            total += nodes;

            std::cout << char('a' + move.from.file()) << char('1' + move.from.rank())
                      << char('a' + move.to.file()) << char('1' + move.to.rank())
                      << ": " << nodes << "\n";
        }
    }

    auto end = high_resolution_clock::now();

    auto ms = duration_cast<milliseconds>(end - start).count();

    std::cout << "\nDepth : " << depth << "\n";
    std::cout << "Nodes : "   << total << "\n";
    std::cout << "Time  : "   << ms    << " ms\n";

    if (ms > 0) std::cout << "NPS   : " << (total * 1000ULL) / ms << "\n";
}

void PerfTest::runPerftest(int depth) {

    MoveGen::init();

    Chess::Board* board = new Chess::Board();
    divide(board, depth);
}
