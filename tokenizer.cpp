#include "tokenizer.hpp"

Tokenizer::Tokenizer(){

}
Tokenizer::Tokenizer(bool mode){
    set_debug(mode);
}

void Tokenizer::set_debug(bool mode){
    debug_mode = mode;
    if(mode){
        cout<<"DEBUG MODE ENABLED" << endl;
    }
}

void Tokenizer::parse(){
    string request;
    char* token;
    
    request.clear();

    if(debug_mode){
        cout << "Enter input to be tokenized: ";
    }
    getline(cin,request);

    token = strtok((char*)request.c_str(), " ");

    while(token != NULL){
        tokens.push_back(token);
        token = strtok(NULL,  " ");
    }

    if(debug_mode){
        cout << "Tokens: \n";
        print_tokens();
    }
}

void Tokenizer::print_tokens(){
    for(auto i : tokens){
        cout << i << endl;
    }
}