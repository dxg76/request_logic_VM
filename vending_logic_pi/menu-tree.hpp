#ifndef MENU_TREE_HPP
#define MENU_TREE_HPP

#include "node.hpp"
#include <iostream>


/*
*Dante Gordon
*
*Class Header file for the menu tree data structure
*/

/*
*Devan Rivera
*
*The menu tree class is used to create a tree structure 
*for the vending machine menu.
*
*/

class Menu_tree{

    public:

        //Default constructor
        Menu_tree();
        //Displays the menu at a specific node
        void selection_menu(Node* node, int level = 0) const;
        //Destructor
        ~Menu_tree();
        //Root Node
        Node* root;

};

#endif //MENU_TREE_HPP