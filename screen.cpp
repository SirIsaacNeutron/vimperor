#include "screen.h"

Screen::Screen(const char* file_name) noexcept 
: file{fopen(file_name, "r")} {
	initscr();
	noecho();
	raw();
}

void Screen::display() const noexcept {
	char character;
	while ((character = getc(file)) != EOF) {
		printw("%c", character);
	}
	refresh();
	move(0, 0); // Set cursor to top-left corner
	getch(); // Pause so the user can see the file
}

Screen::~Screen() {
	endwin();
	fclose(file);
}
