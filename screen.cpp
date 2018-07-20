#include "screen.h"

Screen::Screen() noexcept 
: rows{}, cols{}, file_info_bar{} {
	initscr();
	noecho();
	raw();
	getmaxyx(stdscr, rows, cols);
	scrollok(stdscr, true);

	rows -= 3;
	file_info_bar = newwin(2, cols, rows, 0);
	box(file_info_bar, '|', '-');
}

void Screen::display(std::vector<std::string>::iterator begin,
		std::vector<std::string>::iterator end) const noexcept {
	// We need to clear the screen before displaying it to make sure
	// that scrolling works correctly. If we didn't call clear() here,
	// lines that are bigger than the lines that are about to be displayed
	// on the screen would still partially show up on the screen
	clear();
	std::size_t i = 0;
	// We need to check if i == rows - 1 because if we don't, the screen
	// will omit lines.
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
	box(file_info_bar, '|', '-');
	wrefresh(file_info_bar);
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
	delwin(file_info_bar);
	endwin();
}
