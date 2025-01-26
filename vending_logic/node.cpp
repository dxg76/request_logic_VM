#include "node.hpp"

Node::Node(std::string id, std::string loc, float price, int quantity){

    this->id = id;
    this->loc = loc;
    this->price = price;
    this->quantity = quantity;
    this->left_child = nullptr;
    this->right_child = nullptr;

}

std::string Node::get_id(){

    return id;

}

std::string Node::get_loc(){

    return loc;

}

float Node::get_price(){

    return price;

}

int Node::get_quantity(){

    return quantity;

}


void Node::set_loc(const std::string& loc){

    this->loc = loc;

}

void Node::set_price(float price){

    this->price = price;

}

void Node::set_quantity(int quantity){

    this->quantity = quantity;

}

Node* Node::find_child(const std::string& id) const {

    

}

Node::~Node(){}