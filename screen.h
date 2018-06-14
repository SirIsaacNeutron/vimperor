#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>

class Screen {
	FILE* file;
public:
	Screen(const char* file_name = "") noexcept;
	Screen(const Screen& s) = delete;
	Screen& operator=(const Screen& s) = delete;

	void display() const noexcept;

	~Screen();
};
#endif
