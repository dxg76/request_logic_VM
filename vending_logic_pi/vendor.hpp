#ifndef VENDOR_HPP
#define VENDOR_HPP
#define BUFFER_SIZE 512

#include <cstring>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPi.h>
#include "menu-tree.hpp"
#define BUFFER_SIZE 512

class Vendor{
    public:
    
    Vendor(bool mode);

    /*serial variables*/
    int abstract;
    std::string port_name = "/dev/ttyACM0";
    float total_currency;
    bool card_payment;

    /*vendor vars*/
    short state; //[0 == idle],[1 == select mode], [2 == payment mode], [3 == vend mode]
    bool vend_complete;
    bool list_menu;
    bool confirmation_prompt;
    char motor_control;
    bool voice_control;
    

    /*serial methods*/
    int open_serial(const char* port_name);
    void close_serial();
    bool configure_serial(int speed);

    /*MDB methods*/
    int write_to_MDB(std::string msg);
    std::string read_from_MDB();
    int configure_all();
    int configure_card_reader();
    int configure_coin_mech();
    int configure_bill_validator();
    void print_mdb_response();
    
    /*accept payments*/
    bool check_card_payment(float item_cost);
    float check_coins();
    float check_bills();
    float accept_coins(int hex); //returns 0 if no currency input to machine
    float accept_bills(int hex); //returns 0 if no currency input to machine

    /*vend methods*/
    void set_debug(bool mode); 
    void try_vend(std::string loc, float price);
    bool try_payment(float item_cost);
    
    /*token methods*/
    std::string generate_prompt(Node* current_node);
    std::string check_keywords();
    std::string check_inventory(std::vector<Node*> items);
    std::string confirm_selection();
    std::string wake_up();
    void parse(std::string request, Node* current_node);
    std::string read_tokens(Node* current_node);
    std::string normalize(char* token);
    void empty_tokens();
    void print_tokens();
    std::string get_hex(std::string response);
    float read_hex_code(std::string hex_code);
    char get_vend_code(char row, char col);
    /*token vars*/
    std::vector<std::string> tokens;

    /*menu vars*/
    Menu_tree vendor_menu;
    float selection_price;
    std::string selection_loc;

    /*interrupt vars*/
    volatile int row = 0;
    volatile int col = 0;
    int a_pin = 2;
    int b_pin = 3;
    int c_pin = 4;
    int d_pin = 17;
    int e_pin = 27;
    int f_pin = 22;
    int one_pin = 14;
    int two_pin = 15;
    int three_pin = 18;
    int four_pin = 23;
    int five_pin = 24;
    int six_pin = 25;
    int seven_pin = 8;
    int eight_pin = 7;
    
    /*motor control pins*/
    int m1 = 0;
    int m2 = 1;
    int m3 = 5;
    int m4 = 6;
    int m5 = 12;
    int m6 = 13;

    //ISR rows
    void click_a();
    void click_b();
    void click_c();
    void click_d();
    void click_e();
    void click_f();

    //ISR columns
    void click_one();
    void click_two();
    void click_three();
    void click_four();
    void click_five();
    void click_six();
    void click_seven();
    void click_eight();

    //interrupt methods
    void set_up_interrupts();
    void set_all_gpio();

    //public vars
    const std::string WELCOME_AUDIO = "wav files/Hello_Statement.wav"; 

    private:
    bool debug_mode = false;

    //string constants

    const std::string GREETING_STRING = "Hello my name is MRSTV, I am a vending machine specializing in accessibility.\n"   //mrstv intro greeting 
                                   "To begin, you may call my name and speak a request word, such as chips or candy.\n" 
                                   "For more information use request word help.\n";
    
    //menu prompts
    const std::string RETURN_TO_MAIN = "Respond with one of the selections, or say home to return to main menu.\n";
    //keywords
    const std::string CHIPS_MENU_STRING = "chips"; //opens chip menu
    const std::string CANDY_MENU_STRING = "candy"; //opens candy menu
    const std::string KILL_STRING =  "critical"; //dev code for ending the loop temporarily DG
    const std::string HOME_STRING = "home"; //returns to root menu


};
#endif //VENDOR_HPP