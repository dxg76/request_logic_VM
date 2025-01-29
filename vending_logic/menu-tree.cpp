#include "menu-tree.hpp"

Menu_tree::Menu_tree(){

    root = ned Node("Main Menu");

    Node* candy = new Node("Candy");
    candy->add_child(new Node("KitKat", "", 1.00, 12));
    candy->add_child(new Node("Reese's", "", 1.00, 12));
    candy->add_child(new Node("Hershey's", "", 1.00, 12));
    candy->add_child(new Node("Three Musketeer's", "", 1.00, 12));

    Node* chips = new Node("Chips");
    chips->add_child(new Node("Cool Ranch Dorito's", "", 1.00, 40));
    chips->add_child(new Node("Cheeto's", "", 1.00, 40));
    chips->add_child(new Node("Pretzel's", "", 1.00, 40));
    chips->add_child(new Node("Barbecue Lays", "", 1.00, 40));

    Node* help = new Node("Help");
    help->add_child(new Node("Contact Support"));   //Will implement later
    help->add_child(new Node("Current Selection")); //Will implement later
    //Want to add more option to the help menu

}

void Menu_tree::selection_menu(Node* node, int level) const{

    if(node == nullptr){

        return;

    }

    for(int i = 0; i < level; ++i){

        std::cout << "  ";

    }

    std:: cout << node->get_id() << " - " << node->get_loc() << std::endl;

    for(Node* child : node->get_children()){
        selection_menu(child, level + 1);
    }
    
}   

Node* Menu_tree:get_root() const{

    return root;

}

Menu_tree::~Menu_tree(){

    delete root;

}