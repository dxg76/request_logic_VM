#include "vendor.hpp"

using namespace std;
Vendor::Vendor(){

}
Vendor::Vendor(bool mode){
    set_debug(mode);
    quit_recording.store(false);
    head = new list_node("placeholder",nullptr);
}

void Vendor::set_debug(bool mode){
    debug_mode = mode;
    if(mode){
        cout<<"DEBUG MODE ENABLED" << endl;
    }
}

void Vendor::parse(Node* current_node){
    string request;
    char* token;
    string pass_token;

    request.clear();
    request = get_command();
    //Prompt Generation


    if(debug_mode){
        if(current_node->get_id() == vendor_menu.root->get_id()){
            std::cout << GREETING_STRING << std::endl; //root node

        }else if(current_node->get_price() < 0.1 ){ //checking if menu or selection
            std::cout << RETURN_TO_MAIN << std::endl; //menu node

        }else std::cout << "you have selected " << current_node->get_id() 
                        << " are you sure you would like to purchase this item [Y/N] ?: "
                        << std::endl;
        std:: cout << "Enter input to be tokenized: ";
    }
    
    /*
    getline(cin,request);

    token = strtok((char*)request.c_str(), " ");

    while(token != NULL){
        pass_token = token;
        tokens.push_back(pass_token);
        token = strtok(NULL,  " ");
    }*/
    print_tokens();

}

void Vendor::print_tokens(){
    for(auto i : tokens){
        cout << i << endl;
    }
    cout << "\n\n\n" << endl;
}


string Vendor::read_tokens(Node* current_node){
    if(current_node == vendor_menu.root){
        for(long unsigned int i = 0; i <tokens.size(); ++i){

            if(tokens[i] == CHIPS_MENU_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << CHIPS_MENU_STRING << "\n" << std::endl;
                return CHIPS_MENU_STRING;
            }
            else if(tokens[i] == CANDY_MENU_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << CANDY_MENU_STRING << "\n" << std::endl;
                return CANDY_MENU_STRING;
            }else if(tokens[i] == HELP_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << HELP_STRING << "\n" << std::endl;
                return HELP_STRING;
            }else if(tokens[i] == HOME_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << HOME_STRING << "\n" << std::endl;
                return HOME_STRING;
            }else if(tokens[i] == KILL_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << KILL_STRING << "\n" << std::endl;
                return KILL_STRING;
            }
        }
    }else if(current_node->get_price() < .1){ //in menu searching for item
        for(long unsigned int i = 0; i <tokens.size(); ++i){
            vector<Node*> items = current_node->get_children();
            for(long unsigned int j = 0; j <items.size(); ++j){
                if(tokens[i] == items[j]->get_id()){
                    return tokens[i];
                }
            }
            
        }
        if(debug_mode){
            std::cout << "cannot find the item you are looking for :(\n" << std::endl;
        }
        return "err"; //item not found
    }else{
        if(tokens[0] != "y" && tokens[0] != "n"
        && tokens[0] != "Y" && tokens[0] != "y"){
            if(debug_mode)
                std::cout << "Invalid input: " << tokens[0]  << "please enter [Y/N]: " << std::endl;
            
            return "err";
        }else return tokens[0];
        
    }
    empty_tokens();
    std::cout << "I'm sorry, I didn't quite understand that, please repeat your request."
              << std::endl;
    return "err";
}

void Vendor::empty_tokens(){
    tokens.clear();
    if(debug_mode){
        cout << "Tokens Dumped" << endl;
    }
}

void Vendor::vend(string loc, float price){
    //fill in with vending sequence
    if(debug_mode){
        std::cout << "Vending..." << std::endl;
        std::cout << "vend complete! returning to main menu \n\n" << std::endl;
    }
}

//transcriber method definitions


std::string Vendor::get_command(){
    //audio thread start
    std::thread audio_thread(&Vendor::ma_stream, this);
    
    //quit bool
    bool exit_transcription = false;
    //transcribed text
    std::string text;

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
    quit_recording.store(true);
    audio_thread.join();
    std::cout << "thread joined" <<std::endl;
    
    return text;
}

size_t Vendor::file_read(void * ctx, void * output, size_t read_size) {
    FILE *file = (FILE *) ctx;  // Cast context back to FILE* (since ctx holds a file handle)
    return fread(output, 1, read_size, file); //(buffer, element size, num elements to be read, file stream)
}

bool Vendor::file_eof(void * ctx){
    FILE *file = (FILE *) ctx;  // Cast context back to FILE* (since ctx holds a file handle)
    return feof(file);

}

void Vendor::file_close(void * ctx){
    FILE *file = (FILE *) ctx;  // Cast context back to FILE* (since ctx holds a file handle)
    fclose(file);
}

void Vendor::configure_all(){
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

void Vendor::destroy_all(){
    whisper_free(ctx);
}

//method for generating new ma files
int Vendor::new_file(char* filename, ma_encoder_config encoder_config,  ma_encoder encoder){

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
int Vendor::device_start(ma_device& device, ma_device_config device_config, ma_result& result){

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
void Vendor::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){

    ma_encoder* pEncoder = (ma_encoder*)pDevice->pUserData; //TBD
    MA_ASSERT(pEncoder != NULL); //TBD
    ma_encoder_write_pcm_frames(pEncoder, pInput, frameCount, NULL); //recording frames

    (void)pOutput;
}

//extracts pcm samples from wav
std::vector<float> Vendor::pcm_buster(std::string filename){
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

void Vendor::ma_stream(){
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
    while(!quit_recording){
        
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