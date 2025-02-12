#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdlib.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdio>
/*
Dante Gordon
1/11/24
Mini Audio Capture Demo prelude to full vending algorithm

single file generation complete

working towards multifile compilation
*/
using namespace std::chrono_literals;

int new_file(char* filename, ma_encoder_config encoder_config,  ma_encoder encoder){

    //ENCODER CONFIGURATION
    // initializing 2 channel, wave file, 32bit floating point format encoder with sample rate of 44.1 kHz
    encoder_config = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 1, 16000); 

    //file failure
    if (ma_encoder_init_file(filename, &encoder_config, &encoder) != MA_SUCCESS) {
        std::cout << "Failed to initialize output file." <<std::endl ;
        return -1;
    }
    if(/*debug_mode*/true){
        std::cout << filename << " created successfully" << std::endl;
    }
    return 0;
}

int device_start(ma_device& device, ma_device_config device_config, ma_result& result){

    //device started
    result = ma_device_start(&device);

    //device failure
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        std::cout << "Failed to start device." << std::endl;
        return -3;
    }
    return 0;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){

    ma_encoder* pEncoder = (ma_encoder*)pDevice->pUserData; //TBD
    MA_ASSERT(pEncoder != NULL); //TBD
    ma_encoder_write_pcm_frames(pEncoder, pInput, frameCount, NULL); //recording frames

    (void)pOutput;
}

int main(int argc, char** argv){
    //interaction var
    bool debug_mode = false;
    int index = 0;
    char filenames[10][10]; 
    bool exit = false;
    //Encoder Config
    ma_result result;
    ma_encoder_config encoder_config;
    ma_encoder encoder;  //encodes audio samples to wav files
    ma_device_config device_config;
    ma_device device; //wrapper for interacting with audio devices


    //device initialization
    
    //debug check
    if(argc > 1  && strcmp(argv[1],"-d") == 0){
        debug_mode =  true;
        std::cout <<"Debug mode enabled" << std::endl;
    }

    //populate filenames
    for(int i  = 0; i< 10;++i){
        snprintf(filenames[i], sizeof(filenames[i]), "%d.wav", i + 1); //load array
    }

    if(debug_mode){
        std:: cout << "recording Ctrl^C to stop" << std::endl;
    }
    //Main
    while(!exit){
    
        

        //ENCODER CONFIGURATION
        // initializing 2 channel, wave file, 32bit floating point format encoder with sample rate of 44.1 kHz
        encoder_config = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 1, 16000); 

        //file failure
        if (ma_encoder_init_file(filenames[index], &encoder_config, &encoder) != MA_SUCCESS) {
            std::cout << "Failed to initialize output file." <<std::endl ;
            return -1;
        }
        if(debug_mode){
            std::cout << filenames[index] << " created successfully" << std::endl;
        }
    
 
        //Device Config 
        device_config                  = ma_device_config_init(ma_device_type_capture); //sets up device configuration as audio capture
        device_config.capture.format   = ma_format_f32; // matching encoder configuration
        device_config.capture.channels = 1; // matching encoder channels
        device_config.sampleRate       = 16000; // matching encoder sample rate
        device_config.dataCallback     = data_callback; //assigning data call back method to device configuration
        device_config.pUserData        = &encoder; //TBD
       
        result = ma_device_init(NULL, &device_config, &device); //initializing device
        if (result != MA_SUCCESS) {
            std::cout << "Failed to initialize capture device."<< std::endl;
            return -2;
        } 

        //device started
        result = ma_device_start(&device);

        //device failure
        if (result != MA_SUCCESS) {
            ma_device_uninit(&device);
            std::cout << "Failed to start device." << std::endl;
            return -3;
        }

        
        const auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(5000ms);
        const auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double, std::milli> elapsed = end - start;
        ma_device_uninit(&device);
        ma_encoder_uninit(&encoder);

        index++;
        if(index>9){
            index = 0;
        }
        remove(filenames[index]);
    }
    return 0;

}