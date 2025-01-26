#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>

class Node{
    public:

        Node(std::string id, std::string loc = "", float price = 0.0, int quantity = 0);
        std::string get_id();
        std::string get_loc();
        void set_loc(const std::string& loc);
        float get_price();
        void set_price(float price);
        int get_quantity();
        void set_quantity(int quantity);

        Node* find_child(const std::string& id) const;
    
        ~Node();

    private:

        std::string id;
        std::string loc;
        float price;
        int quantity; 


};

#endif //NODE_HPP