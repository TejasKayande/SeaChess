
#include "window.hpp"

Window::Window() {
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chess");
    _onMenu = false;
}

Window::~Window() {

    CloseWindow(); 
}

Section Window::getBoardSection() const {

    Section sc = { };
    sc.x = 0;
    sc.y = 0;
    sc.width  = BOARD_SIZE;
    sc.height = BOARD_SIZE;

    return sc;
}

Section Window::getStatusSection() const {

    Section sc = { };
    sc.x = 0;
    sc.y = sc.x + BOARD_SIZE;
    sc.width  = BOARD_SIZE;
    sc.height = STATUS_HEIGHT;

    return sc;
}

Section Window::getInformationSection() const {
    
    Section sc = { };

    sc.x = BOARD_SIZE;
    sc.y = 0;
    sc.width  = INFORMATION_WIDTH;
    sc.height = WINDOW_HEIGHT;

    return sc;
}

Section Window::getMenuSection() const {
    
    Section sc = { };

    sc.x = 0;
    sc.y = 0;
    sc.width  = WINDOW_WIDTH;
    sc.height = WINDOW_HEIGHT;

    return sc;
}

void Window::toggleMenu() {

    _onMenu = !_onMenu;
}

bool Window::isOnMenu() const {

    return _onMenu;
}
