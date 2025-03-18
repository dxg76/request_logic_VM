//menu-tree.cpp

#include "menu-tree.hpp"
/*
*Devan Rivera (See ReadME for details)
*Updated 1/29/25
*Updated 2/3/25
*Updated 2/17/25
*Updated 2/18/25
*Updated 3/4/25
*Updated 3/5/25
*/

Menu_tree::Menu_tree(){

    //Creating the root node
    root = new Node("Main Menu","",0,0,"wav files/Begin_Statement.wav");

    //Creating the Candy nodes
    Node* candy = new Node("candy","",0,0,"wav files/what_candy.wav");
        candy->add_child(new Node("kitkat", "A1", 1.00, 12, "wav files/KitKat_Statement.wav")); 
        candy->add_child(new Node("reese's", "A2", 1.00, 12, "wav files/Reeses_Statement.wav"));
        candy->add_child(new Node("butterfinger", "A3", 1.00, 12, "wav files/Butter_Finger_Statement.wav"));
        candy->add_child(new Node("musketeers", "A4", 1.00, 12, "wav files/Musketeers_Statement.wav"));

    //Creating the Chip nodes
    Node* chips = new Node("chips","",0,0,"wav files/what_chips.wav");
        chips->add_child(new Node("doritos", "B1", 1.00, 40, "wav files/Doritos_Statement.wav"));
        chips->add_child(new Node("cheetos", "B2", 1.00, 40, "wav files/Cheetos_Statement.wav"));
        chips->add_child(new Node("pretzels", "B3", 1.00, 40, "wav files/Pretzels_Statement.wav"));
        chips->add_child(new Node("barbecue", "B4", 1.00, 40, "wav files/Barbecue_Statement.wav"));

    //Creating the All nodes
    Node* all = new Node("all");
        all->add_child(new Node("kitkat", "A1", 1.00, 12, "wav files/KitKat_Statement.wav")); 
        all->add_child(new Node("reese's", "A2", 1.00, 12, "wav files/Reeses_Statement.wav"));
        all->add_child(new Node("butterfinger", "A3", 1.00, 12, "wav files/Butter_Finger_Statement.wav"));
        all->add_child(new Node("musketeers", "A4", 1.00, 12, "wav files/Musketeers_Statement.wav"));
        all->add_child(new Node("doritos", "B1", 1.00, 40, "wav files/Doritos_Statement.wav"));
        all->add_child(new Node("cheetos", "B2", 1.00, 40, "wav files/Cheetos_Statement.wav"));
        all->add_child(new Node("pretzels", "B3", 1.00, 40, "wav files/Pretzels_Statement.wav"));
        all->add_child(new Node("barbecue", "B4", 1.00, 40, "wav files/Barbecue_Statement.wav"));

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