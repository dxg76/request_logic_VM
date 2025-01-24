#include "tokenizer.hpp"
using namespace std;
Tokenizer::Tokenizer(){

}
Tokenizer::Tokenizer(bool mode){
    set_debug(mode);
    if(mode == true){
        cout << GREETING_STRING << endl;
    }
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
        //std::cout << token << std::endl;
        tokens.push_back(token);
        token = strtok(NULL,  " ");
    }

    if(debug_mode){
       std::cout << "Tokens: \n";
       print_tokens();
    }
}

void Tokenizer::print_tokens(){
    for(auto i : tokens){
        cout << i << endl;
    }
    cout << "\n\n\n" << endl;
}


string Tokenizer::read_tokens(){
    for(std::string i : tokens){
        if(i == CHIPS_MENU_STRING){
            cout << "Of course here are the chips available ..." << endl;
            return "chip";
        }
        else if(i == CANDY_MENU_STRING){
            cout << "Of course here are the candies available ..." << endl;
            return "candy";
        }else if(i == KILL_STRING){
            return "critical";
        }
    }
    empty_tokens();
    return "I'm sorry, I didn't quite understand that, please repeat your request.";
}

void Tokenizer::empty_tokens(){
    tokens.clear();
    if(debug_mode){
        cout << "Tokens Dumped" << endl;
    }
}