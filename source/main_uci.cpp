
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

struct Command {
    std::string name;
    std::function<void(Args& args)> proc;
};

static std::vector<Command> commands;
static bool running;
static Chess::Board *board;

void uci(Args& args) {
    if (args.size() > 0) std::cout << "uci command should not have any arguments" << std::endl;
    std::cout << "id name SeaChess" << std::endl;
    std::cout << "id author Tejas" << std::endl;
    std::cout << std::endl;
}

void isready(Args& args) {
    if (args.size() > 0) std::cout << "isready command should not have any arguments" << std::endl;
    std::cout << "readyok" << std::endl;
}

void position(Args& args) {
    std::cout << "Non Command Args: " << args.size() << std::endl;
    for (const auto& arg : args) {
        std::cout << arg << " | ";
    }
    std::cout << "position command received" << std::endl;
}

void go(Args& args) {
    std::cout << "Non Command Args: " << args.size() << std::endl;
    for (const auto& arg : args) {
        std::cout << arg << " | ";
    }
    std::cout << std::endl;
    std::cout << "go command received" << std::endl;
}

void quit(Args& args) {
    if (args.size() > 0) std::cout << "isready command should not have any arguments" << std::endl;
    running = false;
}

void initCommands(void) {
    commands = std::vector<Command>({
        { "uci"     , uci      },
        { "isready" , isready  },
        { "position", position },
        { "go"      , go       },
        { "quit"    , quit     }
    });
}

void initBoard(void) {

    board = new Chess::Board();
    board->setFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
}

void printBoard(void) {

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

    std::cout << "  a   b   c   d   e   f   g   h\n";
}

int main(int argc, char** argv) {

    initCommands();
    initBoard();
    running = true;

    printBoard();

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