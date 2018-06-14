#include "screen.h"

Screen::Screen() noexcept {
	initscr();
	noecho();
	raw();
}

void Screen::display(const std::vector<std::string>& file_contents) const noexcept {
	for (const auto& line : file_contents) {
		printw("%s", line.c_str());
		printw("\n");
	}
	refresh();
	move(0, 0); // Set cursor to top-left corner
	getch(); // Pause so the user can see the file
}

Screen::~Screen() {
	endwin();
}
