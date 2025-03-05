#ifndef VENDOR_HPP
#define VENDOR_HPP
#define MINIAUDIO_IMPLEMENTATION
#define BUFFER_SIZE 512

#include <cstring>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "menu-tree.hpp"

class Vendor{
    public:
    
    Vendor(bool mode);

    //serial variables
    int abstract;
    std::string port_name = "/dev/ttyACM0";
    float total_currency;
    //serial methods
    int open_serial(const char* port_name);
    void close_serial();
    bool configure_serial(int speed);

    //MDB methods
    int write_to_MDB(std::string msg);
    std::string read_from_MDB();
    int configure_all();
    int configure_card_reader();
    int configure_coin_mech();
    int configure_bill_validator();

    //accept payments
    bool accept_card_payment(float item_cost);
    float accept_coin_payment(); //returns 0 if no currency input to machine
    float accept_bill_payment(); //returns 0 if no currency input to machine

    //vend methods
    void set_debug(bool mode); 
    void vend(std::string loc, float price);
    bool check_payment(float item_cost);
    
    //token methods
    void parse(std::string request, Node* current_node);
    std::string read_tokens(Node* current_node);
    void empty_tokens();
    void print_tokens();
    std::string get_hex(std::string response);
    float read_coin_code(std::string hex_code);
    float read_bill_code(std::string hex_code);

    //token vars 
    std::vector<std::string> tokens;

    //menu vars
    Menu_tree vendor_menu;
    float selection_price;
    std::string selection_loc;


    private:
    bool debug_mode = false;
    //end transcriber data

    //string constants
    const std::string GREETING_STRING = "Hello my name is MRSTV, I am a vending machine specializing in accessibility.\n"   //mrstv intro greeting 
                                   "To begin, you may call my name and speak a request word, such as chips or candy.\n" 
                                   "For more information use request word help.\n";
    //menu prompts
    const std::string RETURN_TO_MAIN = "Respond with one of the selections, or say home to return to main menu.\n";
    //keywords
    const std::string HELP_STRING = "help"; //mrstv will respond explaining how to interact with him
    const std::string CHIPS_MENU_STRING = "chips"; //opens chip menu
    const std::string CANDY_MENU_STRING = "candy"; //opens candy menu
    const std::string KILL_STRING =  "critical"; //dev code for ending the loop temporarily DG
    const std::string HOME_STRING = "home"; //returns to root menu

};
#endif //VENDOR_HPP