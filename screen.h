#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <vector>
#include <string>

#include "cursor.h"

struct Screen {
	Screen() noexcept;
	Screen(const Screen& s) = delete;
	Screen& operator=(const Screen& s) = delete;

	void display(const std::vector<std::string>& file_contents) const noexcept;
	void move_cursor(const Cursor& cursor) const noexcept;

	~Screen();
};
#endif
