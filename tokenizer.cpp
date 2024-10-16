#include "tokenizer.hpp"

Tokenizer::Tokenizer(){

}
Tokenizer::Tokenizer(bool mode){
    set_debug(mode);
}

void Tokenizer::set_debug(bool mode){
    debug_mode = mode;
    if(mode){
        std::cout<<"DEBUG MODE ENABLED" << std::endl;
    }
}