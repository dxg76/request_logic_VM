X = g++
F = -std=c++14 -Wall -g


main: audio_test.o 
	$(X) -g -o $@ $^

audio_test.o: audio_test.cpp miniaudio.h
	$(X) $(F) -c $<

clean:
	rm -rf *.o *.csv main