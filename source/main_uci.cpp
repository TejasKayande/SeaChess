
#include "core/base.hpp"
#include "core/move.hpp"
#include "core/square.hpp"
#include "core/piece.hpp"
#include "core/board.hpp"
#include "core/movegen.hpp"
#include "core/perft.hpp"

#include <string>
#include <iostream>
#include <functional>

using Args = std::vector<std::string>;

// TODO(Tejas): Maybe this can have the number of arguments it expects.
struct Command {
    std::string name;
    std::function<void(Args& args)> proc;
};

static std::vector<Command> commands;
static bool running;
static std::shared_ptr<Chess::Board> board;

Chess::Square parseSquare(const std::string& str) {

    if (str.size() != 2) return Chess::Square::invalid();

    char file_char = str[0];
    char rank_char = str[1];

    if (file_char < 'a' || file_char > 'h' || rank_char < '1' || rank_char > '8') {
        return Chess::Square::invalid();
    }

    int file = 'h' - file_char;
    int rank = rank_char - '1';

    return Chess::Square(rank, file);
}

void move(std::string move_string) {

    std::string from_str = move_string.substr(0, 2);
    std::string to_str   = move_string.substr(2, 2);
    std::string promo_str;

    Chess::Square from = parseSquare(from_str);
    Chess::Square to   = parseSquare(to_str);

    if (!from.isValid() || !to.isValid()) {
        return;
    }

    MoveList move_list;
    MoveGen::Legal::generateAllMoves(board.get(), move_list);

    for (const Move& move : move_list) {
        if (move.from == from && move.to == to) {
            if (board->makeMove(move)) return;
        }
    }
}

void uci(Args& args) {
    std::cout << "id name SeaChess" << std::endl;
    std::cout << "id author Tejas" << std::endl;
    std::cout << "uciok" << std::endl;
}

void isready(Args& args) {
    std::cout << "readyok" << std::endl;
}

void position(Args& args) {

    if (args[0] == "startpos") {
        board->setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    } else if (args[0] == "fen") {
        std::string fen;
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "moves") break;
            fen += args[i];
            if (i != args.size() - 1) fen += ' ';
        }
        board->setFen(fen);
    } else {
        return;
    }

    for (int i = 0; i < args.size(); i++) {
        if (args[i] == "moves") {
            for (size_t j = i + 1; j < args.size(); j++) {
                move(args[j]);
            }
            break;
        }
    }
}

void go(Args& args) {
    if (args.size() < 1) {
        std::cout << "position command should have at least 1 argument" << std::endl;
        return;
    }

    if (args.size() >= 2 && args[0] == "perft") {
        int depth = std::stoi(args[1]);
        PerfTest::runPerftest(board.get(), depth);
    }
}

void quit(Args& args) {
    
    running = false;
}

void printBoard(Args& args) {

    if (args.size() > 0) std::cout << "isready command should not have any arguments" << std::endl;

    auto pieceToChar = [](const Chess::Piece& piece) -> char {

        if (piece.isEmpty()) return ' ';

        const bool white = piece.color() == Chess::PColor::LIGHT;

        switch (piece.type()) {
            case Chess::PType::PAWN:   return white ? 'P' : 'p';
            case Chess::PType::KNIGHT: return white ? 'N' : 'n';
            case Chess::PType::BISHOP: return white ? 'B' : 'b';
            case Chess::PType::ROOK:   return white ? 'R' : 'r';
            case Chess::PType::QUEEN:  return white ? 'Q' : 'q';
            case Chess::PType::KING:   return white ? 'K' : 'k';
            default:                   return ' ';
        }
    };

    std::cout << '\n';

    for (int rank = 7; rank >= 0; --rank) {
        std::cout << "+---+---+---+---+---+---+---+---+\n";
        for (int file = 7; file >= 0; --file) {
            Chess::Square sq(rank, file);
            std::cout << "| " << pieceToChar(board->getPieceAt(sq)) << " ";
        }

        std::cout << "| "<< rank + 1;
        std::cout << '\n';
    }

    std::cout << "+---+---+---+---+---+---+---+---+\n";

    std::cout << "  a   b   c   d   e   f   g   h\n" << std::endl;

    std::cout << "FEN: " << board->getFen() << std::endl;
}

void initCommands(void) {
    commands = std::vector<Command>({
        { "uci"     , uci        },
        { "isready" , isready    },
        { "position", position   },
        { "go"      , go         },
        { "quit"    , quit       },
        { "d"       , printBoard },
    });
}

void initBoard(void) {
    MoveGen::init();
    board = std::make_shared<Chess::Board>();
}

int main(int argc, char** argv) {

    initCommands();
    initBoard();
    running = true;

    board->setFen("8/P7/8/8/8/8/8/k6K w - - 0 1");

    while (running) {

        std::string input_command;
        std::getline(std::cin, input_command);

        std::string cmd;
        std::stringstream ss(input_command);
        ss >> cmd;

        bool found_cmd = false;

        for (const auto& command : commands) {
            if (command.name == cmd) {
                Args non_cmd_args;

                std::string arg;
                while (ss >> arg) non_cmd_args.push_back(arg);

                command.proc(non_cmd_args);
                found_cmd = true;
                break;
            }
        }

        if (!found_cmd) std::cout << "Unknown command: " << cmd << std::endl;
    }

    return 0;
}