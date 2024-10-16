#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include <cstring>
#include <iostream>
class Tokenizer{
    public:
    void set_debug(bool mode); 
    Tokenizer();
    Tokenizer(bool mode);

    private:
    bool debug_mode = false;
};
#endif //TOKENIZERZ_HPP