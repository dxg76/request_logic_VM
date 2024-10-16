#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
class Tokenizer{
    public:
    void set_debug(bool mode); 
    Tokenizer();
    Tokenizer(bool mode);
    void parse();

    private:
    bool debug_mode = false;
    vector<char*> tokens;
    void print_tokens();

};
#endif //TOKENIZERZ_HPP