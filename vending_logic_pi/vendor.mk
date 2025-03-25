X = g++

F = -std=c++14 -Wall -I./whisper.cpp-master/include -I./whisper.cpp-master/ggml/include #include whisper and ggml headers

L = -L./whisper.cpp-master/build/src -lwhisper -pthread -lm -lsndfile -ldl -l wiringPi #linker flags

main: main.o vendor.o node.o menu-tree.o 
	rm *.wav
	cd whisper.cpp-master && make tiny.en

	$(X) -g -o $@ $^ $(L)

vendor: main.o vendor.o node.o menu-tree.o 
	$(X) -g -o $@ $^ $(L)

main.o: main.cpp vendor.hpp 
	$(X) $(F) -c $<

menu-tree.o: menu-tree.cpp menu-tree.hpp node.hpp
	$(X) $(F) -c $<

node.o: node.cpp node.hpp
	$(X) $(F) -c $<
vendor.o: vendor.cpp vendor.hpp 
	$(X) $(F) -c $<

.PHONY: clean_all
clean_all:
	rm  *.o main vendor *.wav whisper.cpp-master/models/ggml-tiny.en.bin
	rm -rf whisper.cpp-master/build

.PHONY: clean
clean:
	rm  *.o main vendor *.wav 
