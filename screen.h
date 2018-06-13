#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>

class Screen {
	FILE* file;
public:
	Screen(const char* file_name = "") noexcept;

	void display() const noexcept;

	~Screen();
};
#endif
