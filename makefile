vimperor: main.cpp screen.cpp
	g++ -O3 -std=c++14 -Wall -Werror -Wpedantic -Weffc++ screen.cpp main.cpp -o vimperor -lncurses
