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

std::vector <std::string> get_command(std::string filename);
void configure_all();
void destroy_all();



#endif //TRANSCRIBER_HPP
