
#include "window.hpp"

static bool G_onMenu = false;

Window::Section Window::getBoardSection() {

    Section sc = { };
    sc.x = 0;
    sc.y = 0;
    sc.width  = BOARD_SIZE;
    sc.height = BOARD_SIZE;

    return sc;
}

Window::Section Window::getStatusSection() {

    Section sc = { };
    sc.x = 0;
    sc.y = sc.x + BOARD_SIZE;
    sc.width  = BOARD_SIZE;
    sc.height = STATUS_HEIGHT;

    return sc;
}

Window::Section Window::getInformationSection() {
    
    Section sc = { };

    sc.x = BOARD_SIZE;
    sc.y = 0;
    sc.width  = INFORMATION_WIDTH;
    sc.height = WINDOW_HEIGHT;

    return sc;
}

Window::Section Window::getMenuSection() {
    
    Section sc = { };

    sc.x = 0;
    sc.y = 0;
    sc.width  = WINDOW_WIDTH;
    sc.height = WINDOW_HEIGHT;

    return sc;
}

void Window::toggleMenu() {

    G_onMenu = !G_onMenu;
}

bool Window::isOnMenu() {

    return G_onMenu;
}

Window::SectionID Window::getSectionID(int x, int y) {

    SectionID section_id;

    Section scb, scm, sci, scs;
    scb = getBoardSection();
    sci = getInformationSection();
    scs = getStatusSection();
    scm = getMenuSection();

    if (x > scb.x && x < (scb.x + scb.width) && y > scb.y && y < (scb.y + scb.height)) section_id = SectionID::BOARD;
    if (x > scs.x && x < (scs.x + scs.width) && y > scs.y && y < (scs.y + scs.height)) section_id = SectionID::STATUS;
    if (x > sci.x && x < (sci.x + sci.width) && y > sci.y && y < (sci.y + sci.height)) section_id = SectionID::INFORMATION;

    if (isOnMenu())
        if (x > scm.x && x < (scm.x + scm.width) && y > scm.y && y < (scm.y + scm.height)) section_id = SectionID::MENU;

    return section_id;
}

bool Window::onBoard(int x, int y) {
    
    return Window::getSectionID(x, y) == Window::SectionID::BOARD;
}

bool Window::onStatus(int x, int y) {
    
    return Window::getSectionID(x, y) == Window::SectionID::STATUS;
}

bool Window::onInformation(int x, int y) {
    
    return Window::getSectionID(x, y) == Window::SectionID::INFORMATION;
}

Chess::Square Window::getSquare(int x, int y, bool is_board_flipped) {

    if (!Window::onBoard(x, y)) return Chess::Square();

    int rank = y / SQUARE_DIM;
    int file = x / SQUARE_DIM;

    if (is_board_flipped) {
        rank = (Chess::MAX_RANK - 1) - rank;
        file = (Chess::MAX_FILE - 1) - file;
    }

    return Chess::Square(rank, file);
}
