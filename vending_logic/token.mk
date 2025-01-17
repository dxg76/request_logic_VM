X = g++
F = -std=c++14 -Wall -g


main: main.o tokenizer.o
	$(X) -g -o $@ $^

main.o: main.cpp tokenizer.hpp
	$(X) $(F) -c $<

tokenizer.o: tokenizer.cpp tokenizer.hpp
	$(X) $(F) -c $<

clean:
	rm -rf *.o *.csv main