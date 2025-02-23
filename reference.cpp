#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "whisper.h"

// Pseudocode helper to load WAV into float vector, 16 kHz mono
bool load_audio_to_mono_16k(const std::string &filename, std::vector<float> &pcm);

int main() {
    // 1. Initialize Whisper
    whisper_context *ctx = whisper_init("models/ggml-base.en.bin");
    if (!ctx) {
        std::cerr << "Could not load model.\n";
        return 1;
    }

    // 2. Load audio
    std::vector<float> pcmf32;
    if (!load_audio_to_mono_16k("input.wav", pcmf32)) {
        std::cerr << "Failed to load/convert audio.\n";
        return 1;
    }

    // 3. Whisper parameters
    whisper_full_params wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    wparams.n_threads      = 4;
    wparams.translate      = false;  // do not translate to English, just transcribe
    wparams.language       = "en";   // known language
    wparams.print_progress = false;

    // 4. Transcribe
    if (whisper_full(ctx, wparams, pcmf32.data(), pcmf32.size()) != 0) {
        std::cerr << "Error: whisper_full failed.\n";
        whisper_free(ctx);
        return 1;
    }

    // 5. Retrieve text
    int n_segments = whisper_full_n_segments(ctx);
    std::string result;
    for (int i = 0; i < n_segments; i++) {
        const char *text = whisper_full_get_segment_text(ctx, i);
        result += text;
    }

    // 6. Tokenize text & handle your vending logic
    // E.g., detect "SODA", "CANDY", "WATER", etc.
    std::istringstream iss(result);
    std::string token;
    while (iss >> token) {
        // Basic logic to find product
        if (token == "soda") {
            std::cout << "User requested soda.\n";
            // dispenseProduct("soda");
        } else if (token == "water") {
            std::cout << "User requested water.\n";
            // dispenseProduct("water");
        }
        // ... and so on
    }

    // 7. Cleanup
    whisper_free(ctx);
    return 0;
}
