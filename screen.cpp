#include "screen.h"

Screen::Screen() noexcept 
: rows{}, cols{} {
	initscr();
	noecho();
	raw();
	getmaxyx(stdscr, rows, cols);
	scrollok(stdscr, true);
}

void Screen::display(std::vector<std::string>::iterator begin,
		std::vector<std::string>::iterator end) const noexcept {
	// We need to clear the screen before displaying it to make sure
	// that scrolling works correctly. If we didn't call clear() here,
	// lines that are bigger than the lines that are about to be displayed
	// on the screen would still partially show up on the screen
	clear();
	std::size_t i = 0;
	for (auto iterator = begin; i < rows && iterator != end; 
	++iterator, ++i) {
		printw("%s", iterator->c_str());
		if (i != rows - 1) {
			printw("\n");
		}
	}
	for (; i < rows; ++i) {
		printw("~");
		if (i != rows - 1) {
			printw("\n");
		}
	}
	refresh();
}

void Screen::show_first_display(std::vector<std::string>::iterator
		begin, std::vector<std::string>::iterator end) const noexcept {
	display(begin, end);
	move(0, 0); // Move cursor to the top-left corner
}

void Screen::move_cursor(const Cursor& cursor) const noexcept {
	move(cursor.y, cursor.x);		
}

Screen::~Screen() {
	endwin();
}
