//menu-tree.cpp

#include "menu-tree.hpp"
/*
*Devan Rivera (See ReadME for details)
*Updated 1/29/25
*Updated 2/3/25
*Updated 2/17/25
*Updated 2/18/25
*/

Menu_tree::Menu_tree(){

    //Creating the root node
    root = new Node("Main Menu");

    //Creating the Candy nodes
    Node* candy = new Node("candy");
        candy->add_child(new Node("Kit-Kat", "A1", 1.00, 12)); 
        candy->add_child(new Node("Reese's", "A2", 1.00, 12));
        candy->add_child(new Node("Butter", "A3", 1.00, 12));
        candy->add_child(new Node("musketeers", "A4", 1.00, 12));

    //Creating the Chip nodes
    Node* chips = new Node("chips");
        chips->add_child(new Node("Doritos", "B1", 1.00, 40));
        chips->add_child(new Node("Cheetos", "B2", 1.00, 40));
        chips->add_child(new Node("pretzels", "B3", 1.00, 40));
        chips->add_child(new Node("barbecue", "B4", 1.00, 40));

    //Creating the All nodes
    Node* chips = new Node("all");
        all->add_child(new Node("Kit-Kat", "A1", 1.00, 12)); 
        all->add_child(new Node("Reese's", "A2", 1.00, 12));
        all->add_child(new Node("Butter", "A3", 1.00, 12));
        all->add_child(new Node("musketeers", "A4", 1.00, 12));
        all->add_child(new Node("Doritos", "B1", 1.00, 40));
        all->add_child(new Node("Cheetos", "B2", 1.00, 40));
        all->add_child(new Node("pretzels", "B3", 1.00, 40));
        all->add_child(new Node("barbecue", "B4", 1.00, 40));

    //Attaches categories to the root node
    root->add_child(candy);
    root->add_child(chips);
    root->add_child(all);

}

void Menu_tree::selection_menu(Node* node, int level) const{

    if(node == nullptr){
        std::cout << "empty pointer" << std::endl;
        return;

    }

    for(int i = 0; i < level; ++i){

        std::cout << "  ";

    }

    std::cout << node->get_id();

    if(node->get_price() > 0.0){
        
        std::cout << " - $" << node->get_price() << " " << node->get_quantity() << " remaining" << std::endl;

    }

    for(Node* child : node->get_children()){

        selection_menu(child, level + 1);
        
    }

}   

Menu_tree::~Menu_tree(){

    delete root;

}