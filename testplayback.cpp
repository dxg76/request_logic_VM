
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <iostream>
#include <stdlib.h>
#include <sndfile.h>
#include <chrono>
#include<thread>
/*Simple Audio Playback*/

//Callback that feeds audio to the playback device from the decoder
void data_decoder_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
 
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if(pDecoder == NULL){
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;    

}

//Plays the appropriate wav file from the filepath
int play_wav_file(const std::string &filepath){
    //get duration
    SF_INFO sfinfo;
    sf_open(filepath.c_str(), SFM_READ, &sfinfo);
    float duration = (sfinfo.frames/28800);
    int duration_milli = duration*1000;
    //Decoder Config
    ma_result result;
    ma_decoder decoder;
    ma_device_config device_config;
    ma_device decoder_device;

    //Initialize the decoder using the file path
    result = ma_decoder_init_file(filepath.c_str(), NULL, &decoder);

    if(result != MA_SUCCESS){

        std::cerr << "Failed to load file:" << filepath << std::endl;

        return -2;

    }

    //Configure the playback device based on decoder
    device_config = ma_device_config_init(ma_device_type_playback);
    device_config.playback.format   = ma_format_s16; //16 bit signed integer format
    device_config.playback.channels = 1;            //Mono channel due to playback device
    device_config.sampleRate        = 28800;        //Sampling rate set to 1.2*24kHz to adjust for speed of audio
    device_config.dataCallback      = data_decoder_callback;
    device_config.pUserData         = &decoder;

    if(ma_device_init(NULL, &device_config, &decoder_device) != MA_SUCCESS){

        std::cerr << "Failed to initialize playback device." << std::endl;

        ma_decoder_uninit(&decoder);

        return -3;

    }

    if(ma_device_start(&decoder_device) != MA_SUCCESS){

        std::cerr << "Failed to start playback device." << std::endl;

        ma_device_uninit(&decoder_device);
        ma_decoder_uninit(&decoder);

        return -4;

    }

    std::cout << "Playing:" << filepath << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_milli));
    //Uninitialize the playback device and decoder
    ma_device_uninit(&decoder_device);
    ma_decoder_uninit(&decoder);

    return 0;
}

/*
void printSFInfo(const SF_INFO& sfinfo) {
    std::cout << "Audio File Info:\n";
    std::cout << "--------------------------\n";
    std::cout << "Sample Rate  : " << sfinfo.samplerate << " Hz\n";
    std::cout << "Channels     : " << sfinfo.channels << "\n";
    std::cout << "Frames       : " << sfinfo.frames << "\n";
    std::cout << "Sections     : " << sfinfo.sections << "\n";
    std::cout << "Seekable     : " << (sfinfo.seekable ? "Yes" : "No") << "\n";

    // Extract format type and encoding 
    int major_format = sfinfo.format & SF_FORMAT_TYPEMASK;
    int encoding = sfinfo.format & SF_FORMAT_SUBMASK;

    std::cout << "Format Type  : " << major_format << " (Container)\n";
    std::cout << "Encoding     : " << encoding << " (Bit Depth/PCM Type)\n";
}
*/
int main(int argc, char** argv){
    play_wav_file(argv[1]);
    return 0;
}