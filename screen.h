#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <vector>
#include <string>

#include "cursor.h"

// All the functionality related to displaying things on the screen.
// If something Vimperor needs to do involves an ncurses function call,
// it should be done by the Screen struct.
struct Screen {
	std::size_t rows;
	std::size_t cols;

	Screen() noexcept;
	Screen(const Screen& s) = delete;
	Screen& operator=(const Screen& s) = delete;

	void display(std::vector<std::string>::iterator begin,
			std::vector<std::string>::iterator end) const noexcept;
	void show_first_display(std::vector<std::string>::iterator 
			begin, std::vector<std::string>::iterator end) const noexcept;

	void move_cursor(const Cursor& cursor) const noexcept;

	~Screen();
};
#endif
