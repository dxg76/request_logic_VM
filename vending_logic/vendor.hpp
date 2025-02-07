#ifndef VENDOR_HPP
#define VENDOR_HPP
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include "miniaudio.h"

using namespace std;
class Vendor{
    public:
    
    Vendor();
    Vendor(bool mode);

    //token methods
    void parse();
    string read_tokens();
    void empty_tokens();
    
    //audio
    //void record_wav
    //setter
    void set_debug(bool mode); 

    private:
    bool debug_mode = false;
    vector<string> tokens;
    void print_tokens();

    //string constants

    const string GREETING_STRING = "Hello my name is MRSTV, I am a vending machine specializing in accessibility.\n"   //mrstv intro greeting 
                                   "To begin, you may call my name and speak a request word, such as chips or candy.\n" 
                                   "For more information use request word help.\n";
    const string HELP_STRING = "help"; //mrstv will respond explaining how to interact with him
    const string CHIPS_MENU_STRING = "chips"; //opens chip menu
    const string CANDY_MENU_STRING = "candy"; //opens candy menu
    const string KILL_STRING =  "critical"; //dev code for ending the loop temporarily DG
    

};
#endif //VENDOR_HPP