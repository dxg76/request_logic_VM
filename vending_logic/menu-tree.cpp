//menu-tree.cpp

#include "menu-tree.hpp"
/*
*Devan Rivera (See ReadME for details)
*Updated 1/29/25
*Updated 2/3/25
*/

Menu_tree::Menu_tree(){

    //Creating the root node
    root = new Node("Main Menu");

    //Creating the Candy nodes
    Node* candy = new Node("candy");
        candy->add_child(new Node("KitKat", "A1", 1.00, 12)); 
        candy->add_child(new Node("Reese's", "A2", 1.00, 12));
        candy->add_child(new Node("Baby Ruth", "A3", 1.00, 12));
        candy->add_child(new Node("Three Musketeer's", "A4", 1.00, 12));

    //Creating the Chip nodes
    Node* chips = new Node("chips");
        chips->add_child(new Node("Cool Ranch Dorito's", "B1", 1.00, 40));
        chips->add_child(new Node("Cheeto's", "B2", 1.00, 40));
        chips->add_child(new Node("Pretzel's", "B3", 1.00, 40));
        chips->add_child(new Node("Barbecue Lays", "B4", 1.00, 40));

    //Creating the Help nodes
    Node* help = new Node("help");
        help->add_child(new Node("Contact Support"));   //Will implement later
        help->add_child(new Node("Current Selection")); //Will implement later
    //Want to add more option to the help menu

    //Attaches categories to the root node
    root->add_child(candy);
    root->add_child(chips);
    root->add_child(help);

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