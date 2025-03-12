#include <iostream>
#include <sndfile.h>
#include <vector>

void add_padding_to_wav(const std::string &input_file, const std::string &output_file, float padding_seconds) {
    SF_INFO sfinfo;
    SNDFILE *infile = sf_open(input_file.c_str(), SFM_READ, &sfinfo);

    if (!infile) {
        std::cerr << "Error opening input file: " << sf_strerror(infile) << std::endl;
        return;
    }

    // Calculate padding samples
    int padding_samples = static_cast<int>(padding_seconds * sfinfo.samplerate);
    int num_samples = sfinfo.frames * sfinfo.channels;
    
    // Read the original audio data
    std::vector<short> audio_data(num_samples);
    sf_read_short(infile, audio_data.data(), num_samples);
    sf_close(infile);

    // Create new buffer with padding
    std::vector<short> padded_data(num_samples + (padding_samples * sfinfo.channels), 0);

    // Copy original data after padding (appending silence at the start)
    std::copy(audio_data.begin(), audio_data.end(), padded_data.begin() + (padding_samples * sfinfo.channels));

    // Write the new WAV file
    SNDFILE *outfile = sf_open(output_file.c_str(), SFM_WRITE, &sfinfo);
    if (!outfile) {
        std::cerr << "Error opening output file: " << sf_strerror(outfile) << std::endl;
        return;
    }

    sf_write_short(outfile, padded_data.data(), padded_data.size());
    sf_close(outfile);

    std::cout << "Padding added successfully!" << std::endl;
}

int main() {
    add_padding_to_wav("input.wav", "output_padded.wav", 2.0); // Adds 2 seconds of silence
    return 0;
}
