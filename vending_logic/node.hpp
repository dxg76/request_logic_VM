#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>

//Devan was here 1/26-28/25

class Node{
    public:

        Node(const std::string& id, std::string loc = "", float price = 0.0, int quantity = 0);
        
        std::string get_id() const;
        std::string get_loc() const;
        void set_loc(const std::string& loc);
        float get_price() const;
        void set_price(float price);
        int get_quantity() const;
        void set_quantity(int quantity);

        void add_child(Node* child);
        void remove_child(const std::string& id);
        Node* find_child(const std::string& id) const;
        const std::vector<Node*>& get_children() const;

    
        ~Node();

    private:

        const std::string id;
        std::string loc;
        float price;
        int quantity; 
        std::vector<Node*> children;

};

#endif //NODE_HPP