#include "screen.h"

Screen::Screen() noexcept 
: rows{}, cols{}, times_scrolled_down{} {
	initscr();
	noecho();
	raw();
	getmaxyx(stdscr, rows, cols);
	scrollok(stdscr, true);
}

void Screen::display(std::vector<std::string>::iterator begin) const noexcept {
	int i = 0;
	for (auto iterator = begin; i < rows; ++iterator, ++i) {
		printw("%s", (*iterator).c_str());
		printw("\n");
	}
	refresh();
}

void Screen::show_first_display(std::vector<std::string>::iterator
		begin) const noexcept {
	display(begin);
	move(0, 0); // Move cursor to the top-left corner
}

void Screen::move_cursor(const Cursor& cursor) const noexcept {
	move(cursor.y, cursor.x);		
}

void Screen::scroll_down(std::vector<std::string>::iterator
		begin) noexcept {
	display(begin);
	++times_scrolled_down;
}

Screen::~Screen() {
	endwin();
}
