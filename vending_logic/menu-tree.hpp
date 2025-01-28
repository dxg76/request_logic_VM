#ifndef MENU_TREE_HPP
#define MENU_TREE_HPP
#include "node.hpp"


/*
Dante Gordon
1/24/25

Class Header file for the menu tree data structure
*/

//Devan was here 1/28/25 

class Menu_tree{
public:

    Menu_Tree();

    void selection_menu(Node* node, int level = 0) const;
    Node* get_root() const;

    ~Menu_Tree();

private:

    Node* root;

};

#endif //MENU_TREE_HPP

