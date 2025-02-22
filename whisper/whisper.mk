X = g++ 
F = -std=c++14 -Wall -I./whisper.cpp-master/include -I./whisper.cpp-master/ggml/include #include whisper directory
L = -L./whisper.cpp-master/build/src -lwhisper -pthread -lm -lsndfile #linker flags

SRCS = voice_assistant.cpp
O = $(SRCS:.cpp=.o)
T = voice_assistant
$(T): $(O)
	$(X) $(O) -o $(T) $(L)

voice_assistant.o: voice_assistant.cpp miniaudio.h
	$(X) -c $(F) $< -o $@

clean:
	rm -f $(O) $(T) *.wav whisper.cpp-master/models/ggml-base.en.bin
	rm -rf whisper.cpp-master/build