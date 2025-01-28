#include "node.hpp"

Node::Node(const std::string& id, std::string loc, float price, int quantity){

    this->id = id;
    this->loc = loc;
    this->price = price;
    this->quantity = quantity;

}

std::string Node::get_id() const{

    return id;

}

std::string Node::get_loc() const{

    return loc;

}

float Node::get_price() const{

    return price;

}

int Node::get_quantity() const{

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

void Node::add_child(Node* child){

    children.push_back(child);

}

void Node::remove_child(const std::string& id){

    auto it = std::remove_if(children.begin(), children.end(), [&](Node* child){
        return child->get_id() == id;
    })

    if(it != children.end()){

        delete *it;
        children.erase(it, children.end());

    }

}

Node* Node::find_child(const std::string& id) const {

    for(Node* child: children){

        if(child->get_id() == id){
            return child;
        }

    }

    return nullptr;
}

Node::~Node(){

    for(Node* child : children){
        delete child;
    }

}