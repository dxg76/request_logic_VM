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
    string read_tokens();
    void empty_tokens();


    private:
    bool debug_mode = false;
    vector<char*> tokens;
    void print_tokens();

    //string constants

    const string GREETING_STRING = "Hello my name is MRSTV, I am a vending machine specializing in accessibility."   //mrstv intro greeting 
                                    "To begin, you may call my name and speak a request word, such as chips or candy." 
                                    "For more information use request word help.";
    const string HELP_STRING = "help"; //mrstv will respond explaining how to interact with him
    const string CHIPS_MENU_STRING = "chips"; //opens chip menu
    const string CANDY_MENU_STRING = "candy"; //opens candy menu
    const string KILL_STRING =  "critical"; //dev code for ending the loop temporarily DG
    

};
#endif //TOKENIZER_HPP