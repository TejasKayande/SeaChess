
#include "window.hpp"

global bool G_onMenu = false;

Section Window::getBoardSection() {

    Section sc = { };
    sc.x = 0;
    sc.y = 0;
    sc.width  = BOARD_SIZE;
    sc.height = BOARD_SIZE;

    return sc;
}

Section Window::getStatusSection() {

    Section sc = { };
    sc.x = 0;
    sc.y = sc.x + BOARD_SIZE;
    sc.width  = BOARD_SIZE;
    sc.height = STATUS_HEIGHT;

    return sc;
}

Section Window::getInformationSection() {
    
    Section sc = { };

    sc.x = BOARD_SIZE;
    sc.y = 0;
    sc.width  = INFORMATION_WIDTH;
    sc.height = WINDOW_HEIGHT;

    return sc;
}

Section Window::getMenuSection() {
    
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
