#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdlib.h>
#include <string>
#include <iostream>

/*
Dante Gordon
1/11/24
Mini Audio Capture Demo prelude to full vending algorithm

single file generation complete

working towards multifile compilation
*/

int new_file(char* filename, ma_encoder_config encoder_config,  ma_encoder encoder){

    //ENCODER CONFIGURATION
    // initializing 2 channel, wave file, 32bit floating point format encoder with sample rate of 44.1 kHz
    encoder_config = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 2, 44100); 

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

int main(int argc, char** argv)
{
    //interaction var
    bool debug_mode = false;
    
    bool exit = false;
    //Encoder Config
    ma_result result;
    ma_encoder_config encoder_config;
    ma_encoder encoder;  //encodes audio samples to wav files
    ma_device_config device_config;
    ma_device device; //wrapper for interacting with audio devices
    //Device Config 
    device_config                  = ma_device_config_init(ma_device_type_capture); //sets up device configuration as audio capture
    device_config.capture.format   = ma_format_f32; // matching encoder configuration
    device_config.capture.channels = 2; // matching encoder channels
    device_config.sampleRate       = 44100; // matching encoder sample rate
    device_config.dataCallback     = data_callback; //assigning data call back method to device configuration
    device_config.pUserData        = &encoder; //TBD

    //device initialization
    
    //debug check
    if(argc > 1  && strcmp(argv[1],"-d") == 0){
        debug_mode =  true;
        std::cout <<"Debug mode enabled" << std::endl;
    }

    result = ma_device_init(NULL, &device_config, &device); //initializing device
    if (result != MA_SUCCESS) {
        std::cout << "Failed to initialize capture device."<< std::endl;
        return -2;
    }

    //Main
    while(!exit){
        char prompt[100];
        std::cout << "Enter filename :";
        std::cin >> prompt;


        

        int file_result = new_file( prompt,encoder_config, encoder);
        if(file_result != 0){
            return file_result;
        }
 
        int device_result = device_start(device, device_config, result);
        if(device_result != 0){
            return device_result;
        }

        
        char prompt2[100];
        std::cout << "Enter any key to stop recording";
        //std::cin >> prompt2; //placeholder


        ma_device_stop(&device);

        char prompt3[100];
        std::cout << "New File? [y/n]" << std::endl;
        //std::cin >> prompt3;


        if(strcmp(prompt3, "y") != 0){
            if(debug_mode){
            std::cout << prompt3 << std::endl;
            std::cout << "exiting" << std:: endl;
            exit = true;
            }
        }
        
    }
    

    //uninitialization of device and encoder
    ma_device_uninit(&device);
    ma_encoder_uninit(&encoder);

    return 0;
}