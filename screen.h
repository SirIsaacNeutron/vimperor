#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <vector>
#include <string>

#include "cursor.h"

struct Screen {
	int rows;
	int cols;

	Screen() noexcept;
	Screen(const Screen& s) = delete;
	Screen& operator=(const Screen& s) = delete;

	void display(std::vector<std::string>::iterator begin,
			std::vector<std::string>::iterator end) const noexcept;
	void show_first_display(std::vector<std::string>::iterator 
			begin, std::vector<std::string>::iterator end) const noexcept;

	void move_cursor(const Cursor& cursor) const noexcept;
	void scroll_down(std::vector<std::string>::iterator begin) noexcept;

	~Screen();
};
#endif
