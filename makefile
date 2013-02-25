CC       = g++ -Wall -Werror -O3
C11      = -std=c++11

.PHONY: all
all: mines

.PHONY: clean
clean:
	rm -f mines

mines: mines.cpp board.cpp board.hpp cell.hpp statistics.hpp
	$(CC) mines.cpp board.cpp -o $@
