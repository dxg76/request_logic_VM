#include "vendor.hpp"

#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdlib.h>
#include <sndfile.h>

#include "whisper.h" 
#include "miniaudio.h"

/*
*
*Dante Gordon
*Devan Rivera
*Updated 10/13/24
*
*Updated 2/3/25
*Updated 2/6/25
*Updated 2/7/25
*Updated 2/17/25
*Updated 2/18/25
*Updated 3/2/25
*Updated 3/3/25
*Updated 3/5/25
*
*This is the driver file for the MRSTV logic
*/

/*
transcriber vars
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


/*
Transcriber Methods
*/

//method for generating new ma files
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

//starting ma device
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

//callback loop for audio capture
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){

    ma_encoder* pEncoder = (ma_encoder*)pDevice->pUserData; //TBD
    MA_ASSERT(pEncoder != NULL); //TBD
    ma_encoder_write_pcm_frames(pEncoder, pInput, frameCount, NULL); //recording frames

    (void)pOutput;
}

//extracts pcm samples from wav
std::vector<float> pcm_buster(std::string filename){
    SF_INFO sfinfo;
    
    SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (!file) {
        std::cerr << "Error opening file: " << sf_strerror(file) << std::endl;
        return {};
    }
    //printSFInfo(sfinfo); print wav file input
    // Read samples into a float vector
    std::vector<float> samples(sfinfo.frames * sfinfo.channels);
    sf_readf_float(file, samples.data(), sfinfo.frames);

    // Close file
    sf_close(file);

    return samples;

}

void ma_stream(list_node* head){
    //list vars
    int index = 1;
    list_node*tail = head;
    //Encoder Config
    ma_result result;
    ma_encoder_config encoder_config;
    ma_encoder encoder;  //encodes audio samples to wav files
    ma_device_config device_config;
    ma_device device; //wrapper for interacting with audio devices

    //Audio Capture Loop
    while(!exit_recording){
        
        std::string audio_file = std::to_string(index) + ".wav";
        
        //ENCODER CONFIGURATION
        // initializing mono, wave file, 32bit floating point format encoder with sample rate of 44.1 kHz
        encoder_config = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 1, 16000); 

        //file failure
        if (ma_encoder_init_file(audio_file.c_str(), &encoder_config, &encoder) != MA_SUCCESS) {
            std::cout << "Failed to initialize output file." <<std::endl ;
            return;
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
            return;
        } 

        //device started
        result = ma_device_start(&device);

        //device failure
        if (result != MA_SUCCESS) {
            ma_device_uninit(&device);
            std::cout << "Failed to start device." << std::endl;
            return;
        }

        
        //5 second clips
        const auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        const auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double, std::milli> elapsed = end - start;
        ma_device_uninit(&device);
        ma_encoder_uninit(&encoder);

       

        index++;
        if(index > 30){
            index = 1;
        }
        tail->filename = audio_file;
        tail->next_node = new list_node("placeholder",nullptr);
        tail = tail->next_node;

        
    }
    return;


}

void destroy_list(list_node* head){
    while(head != nullptr){
        list_node* temp = head;
        std::string expired_file = head->filename;
        head = head->next_node;
        delete temp; //delete head
        remove(expired_file.c_str());
    }
}
std::string get_command(){
    list_node* head = new list_node("placeholder",nullptr);
    //audio thread start
    exit_recording.store(false);
    std::thread audio_thread(ma_stream,head);
    
    //quit bool
    bool exit_transcription = false;
    //transcribed text
    std::string text;

    std::cout << "starting transcription..."  << std::endl;
    //transcribe loop
    while(!exit_transcription){

        while(head->filename == "placeholder"){
        }

        //get samples
        std::vector<float> samples = pcm_buster(head->filename);

        //transcribe the audio from samples
        if(whisper_full(ctx, full_params, samples.data(), samples.size()) != 0){
            std::cerr << "Error: whisper_full failed.\n";
            whisper_free(ctx);
            std::cerr << "transcription error" << std::endl;
            return NULL;
        }

        //store transcribed text as string
        text = whisper_full_get_segment_text(ctx, 0);

        std::cout << text << std::endl;
        //delete head, delete file and move
        list_node* temp = head;
        std::string expired_file = head->filename;
        head = head->next_node;
        delete temp; //delete head
        remove(expired_file.c_str());

        //check for command
        if(text.find("Mr. Steve") != std::string::npos){
            exit_transcription = true;
        } //end transcribe loop
    }
    exit_recording.store(true);
    audio_thread.join();
    std::cout << "thread joined" <<std::endl;
    
    //destroy list leftovers
    destroy_list(head);
    return text;
}

size_t file_read(void * ctx, void * output, size_t read_size) {
    FILE *file = (FILE *) ctx;  // Cast context back to FILE* (since ctx holds a file handle)
    return fread(output, 1, read_size, file); //(buffer, element size, num elements to be read, file stream)
}

bool file_eof(void * ctx){
    FILE *file = (FILE *) ctx;  // Cast context back to FILE* (since ctx holds a file handle)
    return feof(file);

}

void file_close(void * ctx){
    FILE *file = (FILE *) ctx;  // Cast context back to FILE* (since ctx holds a file handle)
    fclose(file);
}

void configure_all(){
    loader.context = fopen("whisper.cpp-master/models/ggml-base.en.bin", "rb"); // Open model file in raw binary
    loader.read = file_read; //assign read method
    loader.eof = file_eof; //assign eof method
    loader.close = file_close; //assing file close method

    //default context parameters
    params = whisper_context_default_params();

    //initialize whisper context
    ctx = whisper_init_with_params(&loader, params); 

    //check if context was initialized correctly
    if (!ctx) {
        std::cerr << "Failed to initialize Whisper context!" << std::endl;
        file_close(loader.context);
        return;
    }
    std::cout << "\nWhisper context initialized successfully!" << std::endl;

    //full whisper parameters
    full_params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
        full_params.n_threads      = 4; //number of threads running the transcription
        full_params.translate      = false;  // do not translate to English
        full_params.language       = "en";   //language
        full_params.print_progress = false; //show progress
    //setup end

}

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

//Plays the appropraite wav file from the filepath
int play_wav_file(const std::string &filepath){

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

    //Uninitialize the playback device and decoder
    ma_device_uninit(&decoder_device);
    ma_decoder_uninit(&decoder);

    return 0;

}

//main method


int main(int argc, const char** argv){
   
   //setup
    bool debug_mode = false;
    if(argc>1 && strcmp(argv[1], "-d") == 0){
        debug_mode = true;
    }

    //initailize vendor
    Vendor vendor(debug_mode);
    
    //Start at the root ("Main Menu")
    Node* current_node = vendor.vendor_menu.root;
    std::string vendor_result;

    //configure whisper
    configure_all();

    //setup end

    //Plays MR STv's wlecome statement before program starts
    std::string welcome_audio = "wav files/Hello_Statement.wav";
    int welcome_result = play_wav_file(welcome_audio);

    if(welcome_result){
        std::cout << "Audio successfully played." << std::endl; //Debug statement for audio
    }
    else{
        std::cout << "Audio Error." << std::endl;
    }

    //main loop
    while(true){

        //error handling for parse and read
        do{
            vendor.parse(get_command(), current_node);
            vendor_result = vendor.read_tokens(current_node);
        }while(vendor_result == "err");

        //quit sequence
        if(vendor_result == "critical"){
            std::cout << "Exiting program." << std::endl;
            return 0;
        }

        //navigate node based on command
        current_node = current_node->find_child(vendor_result);

        std::string node_audio = current_node->get_audio_path();
        //audio playback for current node
        if(!node_audio.empty()){

            int audio_result = play_wav_file(node_audio);

            if(audio_result){
                std::cout << "Audio successfully played." << std::endl; //Debug statement for audio
            }
            else{
                std::cout << "Audio Error." << std::endl;
            }

        }

        //in menu listing items
        if(current_node != vendor.vendor_menu.root && current_node->get_price() < .1){ 

            std::cout << "---" << vendor_result << " menu---\n" << std::endl; 

            vendor.vendor_menu.selection_menu(current_node, 0);

        }

        //making item selection from sub menu
        else if(current_node != vendor.vendor_menu.root){
            vendor.empty_tokens();
            do{
                vendor.parse(get_command(), current_node);
                vendor_result = vendor.read_tokens(current_node);
            }while(vendor_result == "err");
            if(vendor_result == "y"){
                //Implement payment logic here
                float price = current_node->get_price();

                std::string selected_audio = "wav files/Chosen_Statement.wav";
                int selected_result = play_wav_file(selected_audio);
                if(selected_result == 0){
                    std::cout << "Chosen item audio played successfully." << std::endl;
                } 
                else{
                    std::cout << "Error playing audio." << std::endl;
                }    

                std::cout << "You have selected " << current_node->get_id() << std::endl;
                std::cout << "Please insert " << price << std::endl;

                float payment;
                std::cin >> payment;
                
                if(payment < price){
                    std::cout << "Insufficient funds" << std::endl;

                }
                else{

                    float change = payment - price;
                    std::cout << "Payment accepted. Dispensing: " << current_node->get_id() << std::endl;

                    if(change > 0.0){
                        std::string change_audio = "wav files/Change_Statement.wav";
                        int change_result = play_wav_file(change_audio);
                        if(change_result == 0){
                            std::cout << "Change audio played successfully." << std::endl;
                        } 
                        else{
                            std::cout << "Error playing audio." << std::endl;
                        }

                        std::cout << "Please collect your change: $" << change << std::endl;
                    }

                    current_node->set_quantity(current_node->get_quantity() - 1); //Reduces quantity by 1
                }
                
                vendor.vend(current_node->get_loc(), current_node->get_price()); //vend item define later
                current_node = vendor.vendor_menu.root;
            }else{
                current_node = vendor.vendor_menu.root;
            }

        }

        vendor.empty_tokens();

    }

    //Plays the complete statement after the program ends
    std::cout << "Thank you for using the vending machine." << std::endl;
    std::string vend_complete = "wav files/Complete_Statement.wav";
    int complete_result = play_wav_file(vend_complete);
    if(complete_result == 0){
        std::cout << "Complete_Statement audio played successfully." << std::endl;
    } 
    else{
        std::cout << "Error playing audio." << std::endl;
    }

    return 0;
}