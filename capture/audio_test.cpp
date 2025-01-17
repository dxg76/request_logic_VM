#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

/*
Dante Gordon
1/11/24
Mini Audio Capture Demo prelude to full vending algorithm

single file generation complete

working towards multifile compilation
*/

int new_file(){
    //if(debug_mode){
    //    std::cout << argv[1] << " created successfully" << std::endl;
    //}
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
    bool debug_mode = false;
    char filename[100];

    ma_result result;
    ma_encoder_config encoderConfig;
    ma_encoder encoder;  //encodes audio samples to wav files
    ma_device_config deviceConfig;
    ma_device device; //wrapper for interacting with audio devices

    
    //debug check
    if(argc > 1  && strcmp(argv[1],"-d") == 0){
        debug_mode =  true;
        std::cout <<"Debug mode enabled" << std::endl;
    }

    std::cout << "Enter filename :";

    std::cin >> filename;

    //ENCODER CONFIGURATION
    // initializing 2 channel, wave file, 32bit floating point format encoder with sample rate of 44.kHz
    encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 2, 44100); 

    //file failure
    if (ma_encoder_init_file(filename, &encoderConfig, &encoder) != MA_SUCCESS) {
        std::cout << "Failed to initialize output file." <<std::endl ;
        return -1;
    }


    //file success
    if(debug_mode){
        std::cout << argv[1] << " created successfully" << std::endl;
    }

    //DEVICE CONFIGURATION
    deviceConfig                  = ma_device_config_init(ma_device_type_capture); //sets up device configuration as audio capture
    deviceConfig.capture.format   = encoder.config.format; // matching encoder configuration
    deviceConfig.capture.channels = encoder.config.channels; // matching encoder channels
    deviceConfig.sampleRate       = encoder.config.sampleRate; // matching encoder sample rate
    deviceConfig.dataCallback     = data_callback; //assigning data call back method to device configuration
    deviceConfig.pUserData        = &encoder; //TBD

    result = ma_device_init(NULL, &deviceConfig, &device); //initializing device

    //device failure
    if (result != MA_SUCCESS) {
        printf("Failed to initialize capture device.\n");
        return -2;
    }
    //change!
    //device started
    result = ma_device_start(&device);

    //device failure
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to start device.\n");
        return -3;
    }
    
    char test[20];
    //main
    std::cout << "Enter any key to stop";
    std::cin >> test ;//placeholder
    
    std::cout << "exiting" << std:: endl;

    //uninitialization of device and encoder
    ma_device_uninit(&device);
    ma_encoder_uninit(&encoder);

    return 0;
}