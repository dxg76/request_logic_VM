// Node.cpp
#include "node.hpp"

/*
*Devan Rivera (See ReadME for details)
*Updated 1/28/25
*Updated 1/29/25
*Updated 2/1/25
*Updated 2/3/25
*Updated 2/5/25
*Updated 2/6/25
*Updated 2/17/25
*Updated 3/5/25
*/

Node::Node(const std::string& id,const std::string loc, float price, int quantity, const std::string& audio_path) 
    : id(id), loc(loc), price(price), quantity(quantity), audio_path(audio_path) {}

//Getters
std::string Node::get_id() const{

    return id;

}

std::string Node::get_loc() const{

    return loc;

}

std::string Node::get_audio_path() const{

    return audio_path;

}

float Node::get_price() const{

    return price;

}

int Node::get_quantity() const{

    return quantity;

}

//Setters
void Node::set_loc(const std::string& loc){

    this->loc = loc;

}

void Node::set_price(float price){

    this->price = price;

}

void Node::set_quantity(int quantity){

    this->quantity = quantity;

}

void Node::set_audio_path(const std::string& audio_path){

    this->audio_path = audio_path;

}

//Tree Functions
void Node::add_child(Node* child){

    children.push_back(child);

}

void Node::remove_child(const std::string& target_id){

    //remove_if removes any children with the same id
    auto it = std::remove_if(children.begin(), children.end(), [&](Node* child){

        return child->get_id() == target_id;
    
    });

    //Removed nodes are deleted
        for(auto iter = it; iter != children.end(); ++iter){

            delete *iter;

        }

        children.erase(it, children.end());

}

Node* Node::find_child(const std::string target_id) const{

    for(Node* child: children){
        if(child->get_id() == target_id){
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