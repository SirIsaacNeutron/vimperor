#include "screen.h"

Screen::Screen(const char* file_name) noexcept 
: rows{}, cols{}, file_name{file_name}, file_info_bar{} {
	initscr();
	noecho();
	raw();
	getmaxyx(stdscr, rows, cols);
	scrollok(stdscr, true);

	// We can simply change rows here to make space for more windows.
	// It's the most convenient way I know of to make space for windows
	// without having to change code in Screen::display() or in editor.cpp
	rows -= 4;
	file_info_bar = newwin(3, cols, rows, 0);
}

void Screen::display(std::vector<std::string>::iterator begin,
		std::vector<std::string>::iterator end,
		const Cursor& cursor) const noexcept {
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
	draw_file_info_bar();
	move_cursor(cursor);
}

void Screen::draw_file_info_bar() const noexcept {
	box(file_info_bar, '|', '-');
	if (is_file_modified) {
		mvwprintw(file_info_bar, 1, 1, "%s", file_name.c_str());
		wprintw(file_info_bar, " [+]");
	}
	else {
		mvwprintw(file_info_bar, 1, 1, "%s", file_name.c_str());
		wprintw(file_info_bar, "    "); // Clear out the "modified" sign
	}
	wrefresh(file_info_bar);
}

void Screen::move_cursor(const Cursor& cursor) const noexcept {
	move(cursor.y, cursor.x);		
}

Screen::~Screen() {
	delwin(file_info_bar);
	endwin();
}
