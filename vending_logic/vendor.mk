X = g++
F = -std=c++14 -Wall -g

main: main.o vendor.o menu-tree.o node.o
	$(X) -g -o $@ $^

main.o: main.cpp menu-tree.hpp node.hpp vendor.hpp
	$(X) $(F) -c $<

menu-tree.o: menu-tree.cpp menu-tree.hpp
	$(X) $(F) -c $<

node.o: node.cpp node.hpp
	$(X) $(F) -c $<

vendor.o: vendor.cpp vendor.hpp
	$(X) $(F) -c $<

.PHONY: clean
clean:
	rm -rf *.o *.csv main