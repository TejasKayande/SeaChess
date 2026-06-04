#pragma once

#include <vector>
#include <string>

namespace Menu {

    // TODO(Tejas): I think this doesnt need to be a class. we only need the
    //              item and its index to check it was selected!
    class MenuState {
    public:

        MenuState() {
            _items.push_back("New Game");
            _items.push_back("Load Fen");
            _items.push_back("Themes"); // This needs to redirect to a sub menu
            _items.push_back("Quit");
        }

        ~MenuState(){}

        std::vector<std::string> getItems() const { return _items; }

        void advanceSelection() {

            // NOTE(Tejas): I want to use the % op here, I cant remeber how it works.
            idx_ptr++;
            if (idx_ptr >= _items.size()) idx_ptr = 0;
        };


        void recedeSelection() {

            // NOTE(Tejas): I want to use the % op here, I cant remeber how it works.
            idx_ptr--;
            if (idx_ptr < 0) idx_ptr = _items.size() - 1;
        };

        int getPtr() const { return idx_ptr; }

    private:

        int idx_ptr = 0;

        // NOTE(Tejas): How are we going to handle sub menus? I could have an
        //              Union type that either is a String or a collection of
        //              String (which would mean a sub menu)..?!?!?
        std::vector<std::string> _items;
    };
} // namespace Menu
