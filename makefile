#
# Copyright Erik Bryant (erikbryantology@gmail.com)
#

CC       = g++ -Wall -Werror -Weffc++ -O3
CC_DEBUG = g++ -Wall -Werror -Weffc++ -D_GLIBCXX_DEBUG -g -fprofile-arcs -ftest-coverage -pg
C11      = -std=c++11

.PHONY: all
all: mines

.PHONY: clean
clean:
	rm -f mines

mines: mines.cpp board.cpp board.hpp cell.hpp statistics.hpp
	$(CC) mines.cpp board.cpp -o $@
