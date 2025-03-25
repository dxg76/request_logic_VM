#include "vendor.hpp"
#include <wiringPi.h>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdlib.h>
#include <sndfile.h>
#define MINIAUDIO_IMPLEMENTATION
#include "whisper.h" 
#include "miniaudio.h"


/*
*
*Developers:
*Dante Gordon
*Devan Rivera
*
*This is the driver file for the MRSTV logic and transcription. (See READme for details)
*
*/


/*pin vars*/
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
//methods
void set_up_interrupts();
void set_all_gpio();
void drive_motors(char motor_code);
/*motor control pins*/
int m1 = 0;
int m2 = 1;
int m3 = 5;
int m4 = 6;
int m5 = 12;
int m6 = 13;
int m7 = 19;
int confirm_pin = 16;
volatile char row;
volatile char col;
char motor_control;
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
list_node* head = new list_node("placeholder",nullptr);
list_node* tail = head;
int file_index = 1;
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
    std::vector<float> samples(sfinfo.frames);
    sf_readf_float(file, samples.data(), sfinfo.frames);

    // Close file
    sf_close(file);

    //padding
    int padding = static_cast<int>(.5*sfinfo.samplerate);
    int num_samples = sfinfo.frames;
    std::vector<float> padded_buffer(num_samples + padding*2,0);
    std::copy(samples.begin(), samples.end(), padded_buffer.begin()+padding);

    return padded_buffer;
}

void ma_stream(list_node* head, int recording_length){
    std::cout << "ma_stream()  begin" << std::endl;

    //Encoder Config
    ma_result result;
    ma_encoder_config encoder_config;
    ma_encoder encoder;  //encodes audio samples to wav files
    ma_device_config device_config;
    ma_device device; //wrapper for interacting with audio devices

    //Audio Capture Loop
    while(!exit_recording){
        
        std::string audio_file = std::to_string(file_index) + ".wav";

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
        device_config.pUserData        = &encoder; 
       
        result = ma_device_init(NULL, &device_config, &device); //initializing device
        if (result != MA_SUCCESS) {
            std::cout << "Failed to initialize capture device."<< std::endl;
            return;
        } 

        //device started
        result = ma_device_start(&device);
        if (result != MA_SUCCESS) {
            std::cout << "Failed to start capture device."<< std::endl;
            return;
        } 

        std::cout << "recording..."<< audio_file <<std::endl;


        //device failure
        if (result != MA_SUCCESS) {
            ma_device_uninit(&device);
            std::cout << "Failed to start device." << std::endl;
            return;
        }

        
        //5 second clips
        std::this_thread::sleep_for(std::chrono::milliseconds(recording_length));
        ma_device_uninit(&device);
        ma_encoder_uninit(&encoder);

       

        file_index++;
        if(file_index > 10){
            file_index = 1;
        }
        tail->filename = audio_file;
        tail->next_node = new list_node("placeholder",nullptr);
        tail = tail->next_node;

        
    }
    std::cout << "stopped listening..." << std::endl;

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
    //audio thread start

    //quit bool
    //bool exit_transcription = false;
    //transcribed text
    std::string text;

    //transcribe loop
        while(head->filename == "placeholder"){
            std::cout << "file not ready" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::cout << "exit loop: " << std::endl;
        //get samples
        std::vector<float> samples = pcm_buster(head->filename);

        //transcribe the audio from samples
        std::cout << "transcribing..." << head->filename << std::endl;
        const auto start = std::chrono::high_resolution_clock::now();
        if(whisper_full(ctx, full_params, samples.data(), samples.size()) != 0){
            std::cerr << "Error: whisper_full failed.\n";
            whisper_free(ctx);
            std::cerr << "transcription error" << std::endl;
            return NULL;
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double,std::milli> elapsed = end - start;
        std::cout << "transcription time (secs):  " << elapsed.count()/1000.0 << std::endl;

        //store transcribed text as string
        text = whisper_full_get_segment_text(ctx, 0);

        std::cout << text << std::endl;
        //delete head, delete file and move
        list_node* temp = head;
        std::string expired_file = head->filename;
        head = head->next_node;
        delete temp; //delete head
        remove(expired_file.c_str());

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
    loader.context = fopen("whisper.cpp-master/models/ggml-tiny.en.bin", "rb"); // Open model file in raw binary
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

//Plays the appropriate wav file from the filepath
int play_wav_file(const std::string &filepath){
    //get duration
    SF_INFO sfinfo;
    sf_open(filepath.c_str(), SFM_READ, &sfinfo);
    float frames = sfinfo.frames;
    float duration = (frames/28800);
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

    std::cout << "MRSTV done speaking!" << std::endl;
    return 0;
}
//audio playback end
void list_products(Node* current_node){
    play_wav_file(current_node->get_audio_path());
    std::vector<Node*> products = current_node->get_children();
    for(size_t i = 0; i < products.size(); ++i){
        play_wav_file(products[i]->get_audio_path());
    }
}

void play_confirm(Node* current_node){
    play_wav_file("wav files/Chosen_Statement.wav");
    play_wav_file(current_node->get_audio_path());
    play_wav_file("wav files/Confirm_Deny_Statement.wav");
}
/*
*
*
*
*main method
*
*
*
*/

int main(int argc, const char** argv){
    
    //setup begin 
    //debug
    bool debug_mode = false;
    if(argc>1 && strcmp(argv[1], "-d") == 0){
        debug_mode = true;
    }
    //GPIO
    wiringPiSetupGpio();
    set_all_gpio();
    //initailize vendor
    Vendor vendor(debug_mode);
    int fail_count = 0; //number of fails
    //Start at the root ("Main Menu")
    Node* current_node = vendor.vendor_menu.root;
    std::string vendor_result;
    //configure whisper
    configure_all();
    //setup end
        
    //Plays MR STv's wlecome statement before program starts
    //play_wav_file(vendor.WELCOME_AUDIO);

    //main loop
    while(true){
        
        //standard function loop
        while(!vendor.voice_control){
            current_node = vendor.vendor_menu.root;
            current_node = current_node->find_child("all");
        }

        /*VENDOR STATE 0 IDLE*/
        if(vendor.state == 0){}
        /*VENDOR STATE 1 SELECTION*/
        else if(vendor.state ==1){
            std::string file_path = vendor.generate_prompt(current_node);

            //if submenu
            if(vendor.list_menu){
                list_products(current_node);
                vendor.list_menu = false;
            }
            //if selection made
            else if(vendor.confirmation_prompt){
                play_confirm(current_node);
                vendor.confirmation_prompt = false;
            }
            //other/main
            else    play_wav_file(file_path);


        }
        /*VENDOR STATE 2 Payment*/
        else if(vendor.state == 2){
            play_wav_file("wav files/direct_pay.wav");
            vendor.try_payment(current_node->get_price());

            /*VENDOR STATE 3*/
            //payment has been accepted
            if(vendor.state == 3){
                //reuse bool
                if(vendor.list_menu){
                    play_wav_file("wav files/payment_accepted.wav");
                    play_wav_file("wav files/now_vending.wav");
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    vendor.list_menu = false;
                }
                //Simulate vend with sleep (temporary) DG
                motor_control = vendor.try_vend(current_node->get_loc(), current_node->get_price(),current_node->get_quantity());
                //dispense snack using motors
                if(motor_control != 0){
                    drive_motors(motor_control);
                }
                play_wav_file("wav files/anything_else.wav");
            }

        }
        

        //start recording
        exit_recording.store(false);
        int recording_size_milli;
        if(vendor.state == 0){
            recording_size_milli = 5000;
        }else recording_size_milli = 3000;

        std::thread audio_thread(ma_stream, head, recording_size_milli);
        //Get Input, Tokenize, read
        do{
            vendor.parse(get_command(), current_node);
            const auto start = std::chrono::high_resolution_clock::now();
            vendor_result = vendor.read_tokens(current_node);
            const auto end = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double,std::milli> elapsed = end - start;
            std::cout << "token read time (secs):  " << elapsed.count()/1000.0 << std::endl;
            std::cout << "vendor result: " << vendor_result << std::endl;  
            
            std::cout << "fail count" << fail_count << std::endl
            //fail counter
            if(vendor_result == "err"){
                fail_count +=1;
                if(fail_count == 5){
                    play_wav_file("wav files/try_again");
                }
                if(fail_count == 10){
                    vendor_result = "idle";
                    break;
                }
            }   

        //stop recording
        exit_recording.store(true);
        audio_thread.join();
        std::cout << "audio thread joined!" << std::endl;


        //quit sequence
        if(vendor_result == "critical"){
            std::cout << "Exiting program." << std::endl;
            break;
        }        
        //return to root node
        else if(vendor_result == "home"){
            play_wav_file("wav files/return_home.wav");
            current_node = vendor.vendor_menu.root;
        }
        //go to idle mode
        else if(vendor_result == "idle"){
            play_wav_file("wav files/idle_mode.wav");
            current_node = vendor.vendor_menu.root;
        }
        //exit idle mode
        else if(vendor_result == "awaken"){
            vendor.state = 1;
            play_wav_file("wav files/return_from_idle.wav");
        }
        //navigate node based on command
        else {
            //check if leaf before changing node
            if(!current_node->is_leaf())
                current_node = current_node->find_child(vendor_result);
        }

        //debug out
        std::cout << "Vendor State = "        << vendor.state << std::endl;
        std::cout << "Current Node = "        << current_node->get_id() <<std::endl;
        std::cout << "list_menu = "           << vendor.list_menu << std::endl;
        std::cout << "Confirmation Prompt = " << vendor.confirmation_prompt << std::endl;
    }

    //Plays the complete statement after the program ends
    std::cout << "Thank you for using the vending machine." << std::endl;
    play_wav_file("wav files/exit_statement.wav");


    return 0;
}

//method definitions
void set_up_interrupts(){
    //rows
    if(wiringPiISR(a_pin, INT_EDGE_FALLING, &click_a) != 0)
        std::cout << "Pin: " << a_pin << " interrupt failed." << std::endl;
    if(wiringPiISR(b_pin, INT_EDGE_FALLING, &click_b) != 0)
        std::cout << "Pin: " << b_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(c_pin, INT_EDGE_FALLING, &click_c) != 0)
        std::cout << "Pin: " << c_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(d_pin, INT_EDGE_FALLING, &click_d) != 0)
        std::cout << "Pin: " << d_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(e_pin, INT_EDGE_FALLING, &click_e) != 0)
        std::cout << "Pin: " << e_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(f_pin, INT_EDGE_FALLING, &click_f) != 0)
        std::cout << "Pin: " << f_pin << " interrupt failed." << std::endl;
    //columns
    if (wiringPiISR(one_pin, INT_EDGE_FALLING, &click_one) != 0)
        std::cout << "Pin: " << one_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(two_pin, INT_EDGE_FALLING, &click_two) != 0)
        std::cout << "Pin: " << two_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(three_pin, INT_EDGE_FALLING, &click_three) != 0)
        std::cout << "Pin: " << three_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(four_pin, INT_EDGE_FALLING, &click_four) != 0)
        std::cout << "Pin: " << four_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(five_pin, INT_EDGE_FALLING, &click_five) != 0)
        std::cout << "Pin: " << five_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(six_pin, INT_EDGE_FALLING, &click_six) != 0)
        std::cout << "Pin: " << six_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(seven_pin, INT_EDGE_FALLING, &click_seven) != 0)
        std::cout << "Pin: " << seven_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(eight_pin, INT_EDGE_FALLING, &click_eight) != 0)
        std::cout << "Pin: " << eight_pin << " interrupt failed." << std::endl;
    
}
void set_all_gpio(){
    //rows
    pinMode(a_pin, INPUT);
    pinMode(b_pin, INPUT);
    pinMode(c_pin, INPUT);
    pinMode(d_pin, INPUT);
    pinMode(e_pin, INPUT);
    pinMode(f_pin, INPUT);
    //columns
    pinMode(one_pin, INPUT);
    pinMode(two_pin, INPUT);
    pinMode(three_pin, INPUT);
    pinMode(four_pin, INPUT);
    pinMode(five_pin, INPUT);
    pinMode(six_pin, INPUT);
    pinMode(seven_pin, INPUT);
    pinMode(eight_pin, INPUT);
    //motor control
    pinMode(m1, OUTPUT);
    pinMode(m2, OUTPUT);
    pinMode(m3, OUTPUT);
    pinMode(m4, OUTPUT);
    pinMode(m5, OUTPUT);
    pinMode(m6, OUTPUT);
    pinMode(confirm_pin, INPUT);
    digitalWrite(m1, 0);
    digitalWrite(m2, 0);
    digitalWrite(m3, 0);
    digitalWrite(m4, 0);
    digitalWrite(m5, 0);
    digitalWrite(m6, 0);
    //rows pullups
    pullUpDnControl(a_pin, PUD_UP);
    pullUpDnControl(b_pin, PUD_UP);
    pullUpDnControl(c_pin, PUD_UP);
    pullUpDnControl(d_pin, PUD_UP);
    pullUpDnControl(e_pin, PUD_UP);
    pullUpDnControl(f_pin, PUD_UP);
    //columns pullups
    pullUpDnControl(one_pin, PUD_UP);
    pullUpDnControl(two_pin, PUD_UP);
    pullUpDnControl(three_pin, PUD_UP);
    pullUpDnControl(four_pin, PUD_UP);
    pullUpDnControl(five_pin, PUD_UP);
    pullUpDnControl(six_pin, PUD_UP);
    pullUpDnControl(seven_pin, PUD_UP);
    pullUpDnControl(eight_pin, PUD_UP);
}

//ISR
void click_a(){
    row = 'A';
    std::cout << "Interrupt on row A (GPIO " << a_pin << ")" << std::endl;
}

void click_b(){
    row = 'B';
    std::cout << "Interrupt on row B (GPIO " << b_pin << ")" << std::endl;
}

void click_c(){
    row = 'C';
    std::cout << "Interrupt on row C (GPIO " << c_pin << ")" << std::endl;
}

void click_d(){
    row = 'D';
    std::cout << "Interrupt on row D (GPIO " << d_pin << ")" << std::endl;
}

void click_e(){
    row = 'E';
    std::cout << "Interrupt on row E (GPIO " << e_pin << ")" << std::endl;
}

void click_f(){
    row = 'F';
    std::cout << "Interrupt on row F (GPIO " << f_pin << ")" << std::endl;
}

// ISR Columns
void click_one(){
    col = '1';
    std::cout << "Interrupt on column 1 (GPIO " << one_pin << ")" << std::endl;
}

void click_two(){
    col = '2';
    std::cout << "Interrupt on column 2 (GPIO " << two_pin << ")" << std::endl;
}

void click_three(){
    col = '3';
    std::cout << "Interrupt on column 3 (GPIO " << three_pin << ")" << std::endl;
}

void click_four(){
    col = '4';
    std::cout << "Interrupt on column 4 (GPIO " << four_pin << ")" << std::endl;
}

void click_five(){
    col = '5';
    std::cout << "Interrupt on column 5 (GPIO " << five_pin << ")" << std::endl;
}

void click_six(){
    col = '6';
    std::cout << "Interrupt on column 6 (GPIO " << six_pin << ")" << std::endl;
}

void click_seven(){
    col = '7';
    std::cout << "Interrupt on column 7 (GPIO " << seven_pin << ")" << std::endl;
}

void click_eight(){
    col = '8';
    std::cout << "Interrupt on column 8 (GPIO " << eight_pin  << ")" << std::endl;
}

void drive_motors(char motor_control){
    //output motor code
    digitalWrite(m1,motor_control & 1);
    digitalWrite(m2,(motor_control >> 1) & 1);
    digitalWrite(m3,(motor_control >> 2) & 1);
    digitalWrite(m4,(motor_control >> 3) & 1);
    digitalWrite(m5,(motor_control >> 4) & 1);
    digitalWrite(m6,(motor_control >> 5) & 1);
    
    while(!digitalRead(confirm_pin)){
        std::cout << "vending..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    //reset to default
    digitalWrite(m1, 0);
    digitalWrite(m2, 0);
    digitalWrite(m3, 0);
    digitalWrite(m4, 0);
    digitalWrite(m5, 0);
    digitalWrite(m6, 0);
    std::cout << "done vending!" << std::endl;
}
