#ifndef VENDOR_HPP
#define VENDOR_HPP
#include <cstring>
#include "miniaudio.h"
#include "menu-tree.hpp"

using namespace std;
class Vendor{
    public:
    
    Vendor();
    Vendor(bool mode);
    //vend methods
    void set_debug(bool mode); 
    void vend(string loc, float price);
    
    //token methods
    void parse(Node* current_node);
    string read_tokens(Node* current_node);
    void empty_tokens();
    void print_tokens();

    //token vars 
    vector<string> tokens;

    //menu vars
    Menu_tree vendor_menu;
    float selection_price;
    std::string selection_loc;

    //audio
    //void record_wav
    //setter 

    private:
    bool debug_mode = false;
    
    //string constants
    const string GREETING_STRING = "Hello my name is MRSTV, I am a vending machine specializing in accessibility.\n"   //mrstv intro greeting 
                                   "To begin, you may call my name and speak a request word, such as chips or candy.\n" 
                                   "For more information use request word help.\n";
    //menu prompts
    const string RETURN_TO_MAIN = "Respond with one of the selections, or say home to return to main menu.\n";
    //keywords
    const string HELP_STRING = "help"; //mrstv will respond explaining how to interact with him
    const string CHIPS_MENU_STRING = "chips"; //opens chip menu
    const string CANDY_MENU_STRING = "candy"; //opens candy menu
    const string KILL_STRING =  "critical"; //dev code for ending the loop temporarily DG
    const string HOME_STRING = "home"; //returns to root menu

};
#endif //VENDOR_HPP