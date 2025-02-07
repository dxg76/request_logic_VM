#include "Vendor.hpp"
using namespace std;
Vendor::Vendor(){

}
Vendor::Vendor(bool mode){
    set_debug(mode);
    if(mode == true){
        cout << GREETING_STRING << endl;
    }
}

void Vendor::set_debug(bool mode){
    debug_mode = mode;
    if(mode){
        cout<<"DEBUG MODE ENABLED" << endl;
    }
}

void Vendor::parse(){
    string request;
    char* token;
    string pass_token;
    request.clear();

    if(debug_mode){
        std:: cout << "Enter input to be tokenized: ";
    }

    getline(cin,request);

    token = strtok((char*)request.c_str(), " ");

    while(token != NULL){
        //std::cout << token << std::endl;
        pass_token = token;
        tokens.push_back(pass_token);
        token = strtok(NULL,  " ");
    }

    if(debug_mode){
       std::cout << "Tokens: \n";
       print_tokens();
    }

}

void Vendor::print_tokens(){
    for(auto i : tokens){
        cout << i << endl;
    }
    cout << "\n\n\n" << endl;
}


string Vendor::read_tokens(){
    for(long unsigned int i = 0; i <tokens.size(); ++i){
        if(debug_mode){
            std::cout << "comparing: " << '"' << tokens[i] << '"' << " to: " 
                      << CHIPS_MENU_STRING << ", " 
                      << CANDY_MENU_STRING << ", "
                      << KILL_STRING << "\n" << std::endl;
        }
        if(tokens[i] == CHIPS_MENU_STRING){
            cout << "Of course here are the chips available ..." << endl;
            return "chip";
        }
        else if(tokens[i] == CANDY_MENU_STRING){
            cout << "Of course here are the candies available ..." << endl;
            return "candy";
        }else if(tokens[i] == KILL_STRING){
            return "critical";
        }
    }
    empty_tokens();
    return "I'm sorry, I didn't quite understand that, please repeat your request.";
}

void Vendor::empty_tokens(){
    tokens.clear();
    if(debug_mode){
        cout << "Tokens Dumped" << endl;
    }
}