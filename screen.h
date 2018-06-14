#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <vector>
#include <string>

struct Screen {
	Screen() noexcept;
	Screen(const Screen& s) = delete;
	Screen& operator=(const Screen& s) = delete;

	void display(const std::vector<std::string>& file_contents) const noexcept;

	~Screen();
};
#endif
