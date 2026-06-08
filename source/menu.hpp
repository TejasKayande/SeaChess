#pragma once

#include <vector>
#include <string>
#include <functional>

namespace Menu {

    class Menu; // NOTE(Tejas): This is soo cool

    struct MenuItem {
        std::string label;
        std::function<void()> proc;
        Menu *sub_menu;
    };

    class Menu {

    public:

        Menu() = default;
        Menu(std::string title, std::vector<MenuItem> items) : m_title(title), m_items(items) {}
        ~Menu() = default;

        void next() { m_ptr = (m_ptr + 1) % m_items.size(); }
        void prev() { m_ptr = (m_ptr - 1 + m_items.size()) % m_items.size(); }

        std::vector<MenuItem> getItems() const { return m_items; }

        std::string getTitle() const { return m_title; }

        int getPtr() const { return m_ptr; }

    private:

        int m_ptr = 0;
        std::string m_title;
        std::vector<MenuItem> m_items;
    };

    // class MenuState {

    // public:
    //     enum MenuScreen {
    //         MAIN_MENU,
    //         THEME_MENU
    //     };

    // public:

    //     MenuState() {
    //         m_items.push_back("New Game");
    //         m_items.push_back("Load Fen");
    //         m_items.push_back("Themes"); // This needs to redirect to a sub menu
    //         m_items.push_back("Quit");

    //         m_theme_items = {
    //             "Default",
    //             "Classic Wood",
    //             "Slate Blue",
    //             "Emerald"
    //         };
    //     }

    //     ~MenuState(){}

    //     std::vector<std::string> getItems() const { 
    //         if (m_screen == MenuScreen::THEME_MENU) return m_theme_items;
    //         return m_items; 
    //     }

    //     void advanceSelection() {

    //         // NOTE(Tejas): I want to use the % op here, I cant remeber how it works.
    //         m_idx_ptr++;
    //         if (m_idx_ptr >= m_items.size()) m_idx_ptr = 0;
    //     };


    //     void recedeSelection() {

    //         // NOTE(Tejas): I want to use the % op here, I cant remeber how it works.
    //         m_idx_ptr--;
    //         if (m_idx_ptr < 0) m_idx_ptr = m_items.size() - 1;
    //     };

    //     int getPtr() const { return m_idx_ptr; }

    //     void changeScreen(MenuScreen new_screen) {
    //         m_screen = new_screen;
    //         m_idx_ptr = 0;
    //     }

    //     MenuScreen getMenuScreen() const { return m_screen; }

    // private:

    //     int m_idx_ptr = 0;
    //     MenuScreen m_screen = MenuScreen::MAIN_MENU;

    //     // NOTE(Tejas): How are we going to handle sub menus? I could have an
    //     //              Union type that either is a String or a collection of
    //     //              String (which would mean a sub menu)..?!?!?
    //     std::vector<std::string> m_items;
    //     std::vector<std::string> m_theme_items;
    // };
} // namespace Menu
