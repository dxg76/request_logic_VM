//menu-tree.cpp

#include "menu-tree.hpp"

/*
*Devan Rivera (See ReadME for details)
*/

Menu_tree::Menu_tree(){

    //Creating the root node
    root = new Node("Main Menu","",0,{},"wav files/Begin_Statement.wav");

    //Creating the Candy nodes
    Node* candy = new Node("candy","",0,{},"wav files/what_candy.wav");
        candy->add_child(new Node("kitkat", "E7", 1.00, {4,4}, "wav files/KitKat_Statement.wav")); 
        candy->add_child(new Node("reese's", "E1", 1.00, {4,4}, "wav files/Reeses_Statement.wav"));
        candy->add_child(new Node("butterfinger", "E3", 1.00, {4,4}, "wav files/Butter_Finger_Statement.wav"));
        candy->add_child(new Node("crunch", "E5", 1.00, {10,10,10,10}, "wav files/Musketeers_Statement.wav"));

    //Creating the Chip nodes
    Node* chips = new Node("chips","",0,{},"wav files/what_chips.wav");
        chips->add_child(new Node("doritos", "A1", 1.00, {10,10,10,10}, "wav files/Doritos_Statement.wav"));
        chips->add_child(new Node("cheetos", "B1", 1.00, {10,10,10,10}, "wav files/Cheetos_Statement.wav"));
        chips->add_child(new Node("pretzels", "D1", 1.00, {10,10,10,10}, "wav files/Pretzels_Statement.wav"));
        chips->add_child(new Node("barbecue", "C1", 1.00, {10,10,10,10}, "wav files/Barbecue_Statement.wav"));

    //Creating the All nodes
    Node* all = new Node("all");
        all->add_child(new Node("kitkat", "E7", 1.00, {4,4}, "wav files/KitKat_Statement.wav")); 
        all->add_child(new Node("reese's", "E1", 1.00, {4,4}, "wav files/Reeses_Statement.wav"));
        all->add_child(new Node("butterfinger", "E3", 1.00, {4,4}, "wav files/Butter_Finger_Statement.wav"));
        all->add_child(new Node("crunch", "E5", 1.00, {4,4}, "wav files/Musketeers_Statement.wav"));
        all->add_child(new Node("doritos", "A1", 1.00, {10,10,10,10}, "wav files/Doritos_Statement.wav"));
        all->add_child(new Node("cheetos", "B1", 1.00, {10,10,10,10}, "wav files/Cheetos_Statement.wav"));
        all->add_child(new Node("pretzels", "D1", 1.00, {10,10,10,10}, "wav files/Pretzels_Statement.wav"));
        all->add_child(new Node("barbecue", "C1", 1.00, {10,10,10,10}, "wav files/Barbecue_Statement.wav"));

    //Attaches categories to the root node
    root->add_child(candy);
    root->add_child(chips);
    root->add_child(all);

}

void Menu_tree::selection_menu(Node* node, int level) const{
    std::vector<int> stock = node->get_quantity();
    int quantity = 0;
    
    for(size_t i = 0; i < stock.size(); ++i){
        quantity += stock[i];
    }

    if(node == nullptr){
        std::cout << "empty pointer" << std::endl;
        return;
    }

    for(int i = 0; i < level; ++i){
        std::cout << "  ";
    }

    std::cout << node->get_id();

    if(node->get_price() > 0.0){
        std::cout << " - $" << node->get_price() << " " << quantity << " remaining" << std::endl;
    }

    for(Node* child : node->get_children()){
        selection_menu(child, level + 1);    
    }
}   

Menu_tree::~Menu_tree(){
    delete root;
}