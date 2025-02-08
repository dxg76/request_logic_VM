X = g++ 
F = -std=c++14 -Wall -I./whisper.cpp-master/include -I./whisper.cpp-master/ggml/include #include whisper directory
L = -L./whisper.cpp-master/build/src -lwhisper -pthread -lm #linker flags

SRCS = voice_assistant.cpp
O = $(SRCS:.cpp=.o)
T = voice_assistant
$(T): $(O)
	$(X) $(O) -o $(T) $(L)

%.o: %.cpp
	$(X) -c $(F) $< -o $@

clean:
	rm -f $(O) $(T)