// Node.cpp
#include "node.hpp"

/*
*Devan Rivera (See ReadME for details)
*/

Node::Node(const std::string& id,const std::string loc, float price, std::vector<int> quantity, const std::string& audio_path) 
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
std::vector<int> Node::get_quantity() const{

    return quantity;

}

//Setters
void Node::set_loc(const std::string& loc){

    this->loc = loc;

}

void Node::set_price(float price){

    this->price = price;

}

void Node::set_quantity(std::vector<int> quantity){

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

bool Node::is_leaf(){
    if(this->children.size() == 0){
        return true;
    }else return false;
}