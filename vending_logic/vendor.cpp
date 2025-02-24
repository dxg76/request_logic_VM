#include "vendor.hpp"

using namespace std;
Vendor::Vendor(){

}
Vendor::Vendor(bool mode){
    set_debug(mode);
}

void Vendor::set_debug(bool mode){
    debug_mode = mode;
    if(mode){
        cout<<"DEBUG MODE ENABLED" << endl;
    }
}

void Vendor::parse(Node* current_node){
    string request;
    char* token;
    string pass_token;

    request.clear();
    
    //Prompt Generation


    if(debug_mode){
        if(current_node->get_id() == vendor_menu.root->get_id()){
            std::cout << GREETING_STRING << std::endl; //root node

        }else if(current_node->get_price() < 0.1 ){ //checking if menu or selection
            std::cout << RETURN_TO_MAIN << std::endl; //menu node

        }else std::cout << "you have selected " << current_node->get_id() 
                        << " are you sure you would like to purchase this item [Y/N] ?: "
                        << std::endl;
        std:: cout << "Enter input to be tokenized: ";
    }
    
    /*
    getline(cin,request);

    token = strtok((char*)request.c_str(), " ");

    while(token != NULL){
        pass_token = token;
        tokens.push_back(pass_token);
        token = strtok(NULL,  " ");
    }*/
    print_tokens();

}

void Vendor::print_tokens(){
    for(auto i : tokens){
        cout << i << endl;
    }
    cout << "\n\n\n" << endl;
}


string Vendor::read_tokens(Node* current_node){
    if(current_node == vendor_menu.root){
        for(long unsigned int i = 0; i <tokens.size(); ++i){

            if(tokens[i] == CHIPS_MENU_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << CHIPS_MENU_STRING << "\n" << std::endl;
                return CHIPS_MENU_STRING;
            }
            else if(tokens[i] == CANDY_MENU_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << CANDY_MENU_STRING << "\n" << std::endl;
                return CANDY_MENU_STRING;
            }else if(tokens[i] == HELP_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << HELP_STRING << "\n" << std::endl;
                return HELP_STRING;
            }else if(tokens[i] == HOME_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << HOME_STRING << "\n" << std::endl;
                return HOME_STRING;
            }else if(tokens[i] == KILL_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << KILL_STRING << "\n" << std::endl;
                return KILL_STRING;
            }
        }
    }else if(current_node->get_price() < .1){ //in menu searching for item
        for(long unsigned int i = 0; i <tokens.size(); ++i){
            vector<Node*> items = current_node->get_children();
            for(long unsigned int j = 0; j <items.size(); ++j){
                if(tokens[i] == items[j]->get_id()){
                    return tokens[i];
                }
            }
            
        }
        if(debug_mode){
            std::cout << "cannot find the item you are looking for :(\n" << std::endl;
        }
        return "err"; //item not found
    }else{
        if(tokens[0] != "y" && tokens[0] != "n"
        && tokens[0] != "Y" && tokens[0] != "y"){
            if(debug_mode)
                std::cout << "Invalid input: " << tokens[0]  << "please enter [Y/N]: " << std::endl;
            
            return "err";
        }else return tokens[0];
        
    }
    empty_tokens();
    std::cout << "I'm sorry, I didn't quite understand that, please repeat your request."
              << std::endl;
    return "err";
}

void Vendor::empty_tokens(){
    tokens.clear();
    if(debug_mode){
        cout << "Tokens Dumped" << endl;
    }
}

void Vendor::vend(string loc, float price){
    //fill in with vending sequence
    if(debug_mode){
        std::cout << "Vending..." << std::endl;
        std::cout << "vend complete! returning to main menu \n\n" << std::endl;
    }
}
