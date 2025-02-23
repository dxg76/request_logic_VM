#ifndef TRANSCRIBER_HPP
#define TRANSCRIBER_HPP
#define MINIAUDIO_IMPLEMENTATION

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

#include "whisper.h" 
#include "miniaudio.h"


/*
declaration for transcriber methods using whisper.cpp
*/
//linked list struct
struct list_node{
    std::string filename;
    list_node* next_node;
    list_node(std::string filename) : filename(filename), next_node(nullptr) {}
    list_node(std::string filename, list_node* next_node) : filename(filename), next_node(next_node) {}
};

//whisper context and params
whisper_model_loader loader;
whisper_full_params full_params;
struct whisper_context_params params;
struct whisper_context* ctx;

//atomic bool to quit recording
std::atomic<bool> exit_recording(false);
int list_size = 0;
list_node* head = new list_node("placeholder",nullptr);

size_t file_read(void * ctx, void * output, size_t read_size);
bool file_eof(void * ctx);
void file_close(void * ctx);

std::string get_command();
void ma_stream();
int device_start(ma_device& device, ma_device_config device_config, ma_result& result);
int new_file(char* filename, ma_encoder_config encoder_config,  ma_encoder encoder);
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
void configure_all();
void destroy_all();
std::vector<float> pcm_buster(std::string filename);


#endif //TRANSCRIBER_HPP
