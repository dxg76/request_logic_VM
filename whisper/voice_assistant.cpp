#include <iostream>
#include "whisper.h" 
#include <fstream>
#include <sndfile.h>
#include <vector>
#include <string>

//print info about wave file
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

//extracts pcm samples from wav
std::vector<float> pcm_buster(std::string filename){
    SF_INFO sfinfo;
    
    SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (!file) {
        std::cerr << "Error opening file: " << sf_strerror(file) << std::endl;
        return {};
    }
    printSFInfo(sfinfo);
    // Read samples into a float vector
    std::vector<float> samples(sfinfo.frames * sfinfo.channels);
    sf_readf_float(file, samples.data(), sfinfo.frames);

    // Close file
    sf_close(file);

    return samples;

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
int main(){

    std::vector<float> samples = pcm_buster("1.wav");
    whisper_model_loader loader; //loader structs
        loader.context = fopen("whisper.cpp-master/models/ggml-base.en.bin", "rb"); // Open model file in raw binary
        loader.read = file_read; //assign read method
        loader.eof = file_eof; //assign eof method
        loader.close = file_close; //assing file close method

    //default context parameters
    struct whisper_context_params params = whisper_context_default_params();

    struct whisper_context* ctx = whisper_init_with_params(&loader, params); //initialize whisper returns a pointer to context based on params

    if (!ctx) {
        std::cerr << "Failed to initialize Whisper context!" << std::endl;
        file_close(loader.context);
        return -1;
    }
    std::cout << "\nWhisper context initialized successfully!" << std::endl;

    whisper_full_params full_params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    full_params.n_threads      = 4;
    full_params.translate      = false;  // do not translate to English, just transcribe
    full_params.language       = "en";   // known language
    full_params.print_progress = false;

    //transcribe the audio from samples
    if(whisper_full(ctx, full_params, samples.data(), samples.size()) != 0){
        std::cerr << "Error: whisper_full failed.\n";
        whisper_free(ctx);
        return -1;
    }

    int segments =  whisper_full_n_segments(ctx);
    std::string script;
    for (int i = 0; i < segments; i++) {
        const char *text = whisper_full_get_segment_text(ctx, i);
        script += text;
    }
    std::cout <<script << std::endl;

    

    // Cleanup
    whisper_free(ctx);

    return 0;
    
}