vimperor: main.cpp screen.cpp editor.cpp
	g++ -O3 -std=c++14 -Wall -Werror -Wpedantic -Weffc++ screen.cpp editor.cpp main.cpp -o vimperor -lncurses
