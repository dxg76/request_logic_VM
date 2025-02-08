#include <iostream>
#include "whisper.h" 
#include <fstream>

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
    whisper_model_loader loader;
        loader.context = fopen("whisper.cpp-master/models/ggml-base.en.bin", "rb"); // Open file
        loader.read = file_read;
        loader.eof = file_eof;
        loader.close = file_close;
    struct whisper_context_params params = whisper_context_default_params();

    struct whisper_context* ctx = whisper_init_with_params(&loader, params);

    if (!ctx) {
        std::cerr << "Failed to initialize Whisper context!" << std::endl;
        file_close(loader.context);
        return -1;
    }

    std::cout << "/nWhisper context initialized successfully!" << std::endl;

    // Cleanup
    whisper_free(ctx);
    
}