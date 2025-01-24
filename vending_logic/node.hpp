#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include<string>
class Node{
    public:
    Node(std::string id);
    std::string get_id();
    std::string get_loc();
    float get_price();
    Node* find_child();

    private:
    std::string id;
    std::string loc;
    float price;
    std::vector<Node*> children;
};

#endif //NODE_HPP