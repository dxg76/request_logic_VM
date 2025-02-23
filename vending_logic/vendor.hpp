#ifndef VENDOR_HPP
#define VENDOR_HPP
#define MINIAUDIO_IMPLEMENTATION
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdlib.h>
#include <sndfile.h>

#include "miniaudio.h"
#include "menu-tree.hpp"
#include "whisper.h" 


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

    //linked list struct
    struct list_node{
        std::string filename;
        list_node* next_node;
        list_node(std::string filename) : filename(filename), next_node(nullptr) {}
        list_node(std::string filename, list_node* next_node) : filename(filename), next_node(next_node) {}
    };
    //list head
    list_node* head;

    private:
    bool debug_mode = false;
    /*
    declaration for transcriber methods and vars using whisper.cpp
    */
   
    //audio
    static size_t file_read(void * ctx, void * output, size_t read_size);
    static bool file_eof(void * ctx);
    static void file_close(void * ctx);
    void ma_stream();

    //whisper context and params
    whisper_model_loader loader;
    whisper_full_params full_params;
    struct whisper_context_params params;
    struct whisper_context* ctx;

    //atomic bool to quit recording
    std::atomic<bool> quit_recording;
    int list_size = 0;
    
    std::string get_command();
    
    int device_start(ma_device& device, ma_device_config device_config, ma_result& result);
    int new_file(char* filename, ma_encoder_config encoder_config,  ma_encoder encoder);
    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    void configure_all();
    void destroy_all();
    std::vector<float> pcm_buster(std::string filename);
    //end transcriber data

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