#ifndef MENU_TREE_HPP
#define MENU_TREE_HPP

#include "node.hpp"
#include <iostream>


/*
*Dante Gordon
*1/24/25
*
*Class Header file for the menu tree data structure
*/

/*
*Devan Rivera
*1/28/25
*
*The menu tree class is used to create a tree structure 
*for the vending machine menu.
*
*Updated 1/29/25
*Updated 2/1/25
*Updated 2/3/25
*/

class Menu_tree{

    public:

        //Default constructor
        Menu_tree();

        //Displays the menu at a specific node
        void selection_menu(Node* node, int level = 0) const;

        //Returns root node
        Node* get_root() const;

        //Destructor
        ~Menu_tree();

    private:

        Node* root;

};

#endif //MENU_TREE_HPP