X = g++
F = -std=c++14 -Wall -g


main: main.o vendor.o
	$(X) -g -o $@ $^

main.o: main.cpp vendor.hpp
	$(X) $(F) -c $<

vendor.o: vendor.cpp vendor.hpp
	$(X) $(F) -c $<


.phony = clean
clean:
	rm -rf *.o *.csv main