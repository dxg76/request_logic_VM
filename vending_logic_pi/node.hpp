#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm> //need to identify

/*
*Devan Rivera 1/28/25 
*
*Node class is used to create a tree structure 
*for the vending machine menu. Each node has information 
*stored and can be manipulated.
*
*Updated 1/29/25
*Updated 2/1/25
*Updated 2/3/25
*Updated 3/5/25
*/

class Node{
    public:

        //constructor
        Node(const std::string& id,  const std::string loc = "", float price = 0.0, std::vector<int> quantity = {0,0,0,0}, const std::string& audio_path = "");
        
        //Accessors
        std::string get_id() const;
        std::string get_loc() const;
        std::string get_audio_path() const;
        float get_price() const;
        std::vector<int> get_quantity() const;
        const std::vector<Node*>& get_children() const{return children;}

        //Mutators
        void set_loc(const std::string& loc);
        void set_quantity(std::vector<int> quantity);
        void set_price(float price);
        void set_audio_path(const std::string& audio_path);

        //Functions to manipulate children
        void add_child(Node* child);
        void remove_child(const std::string& target_id);
        Node* find_child(const std::string target_id) const;

        //Destructor
        ~Node();
        
        //identifer
        bool is_leaf();
    private:

        std::string id;
        std::string loc;
        float price;
        std::vector<int> quantity; 
        std::vector<Node*> children;
        std::string audio_path;

};

#endif //NODE_HPP