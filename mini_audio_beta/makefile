X = g++
F = -std=c++14 -Wall -g


main: main.o 
	$(X) -g -o $@ $^

main.o: main.cpp miniaudio.h
	$(X) $(F) -c $<

clean:
	rm -rf *.o *.csv main