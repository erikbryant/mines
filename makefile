#
# Copyright Erik Bryant (erikbryantology@gmail.com)
# Gnu All-Permissive http://www.gnu.org/philosophy/license-list.html#GNUAllPermissive
#

CC       = g++ -Wall -Werror -Weffc++ -O3
CC_DEBUG = g++ -Wall -Werror -Weffc++ -D_GLIBCXX_DEBUG -g -fprofile-arcs -ftest-coverage -pg
C11      = -std=c++11
CPPCHECK = ../cppcheck-1.58/cppcheck

.PHONY: all
all: mines

.PHONY: clean
clean:
	rm -f mines

mines: mines.cpp board.cpp board.hpp cell.hpp statistics.hpp
	$(CPPCHECK) mines.cpp board.cpp
	$(CC) mines.cpp board.cpp -o $@
