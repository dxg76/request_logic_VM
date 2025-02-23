#include "transcriber.hpp"

std::string get_command(){
    //audio thread start
    std::thread audio_thread(ma_stream);
    
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
    exit_recording.store(true);
    audio_thread.join();
    std::cout << "thread joined" <<std::endl;
    
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

void destroy_all(){
    whisper_free(ctx);
}

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

void ma_stream(){
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