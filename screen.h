#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <vector>
#include <string>

#include "cursor.h"

// All the functionality related to displaying things on the screen.
// If something Vimperor needs to do involves an ncurses function call,
// it should be done by the Screen struct.
class Screen {
public:
	std::size_t rows;
	std::size_t cols;
	bool is_file_modified{false};

	Screen(const char* file_name) noexcept;
	Screen(const Screen& s) = delete;
	Screen& operator=(const Screen& s) = delete;

	void display(std::vector<std::string>::const_iterator begin,
			std::vector<std::string>::const_iterator end,
			const Cursor& cursor) const noexcept;

	~Screen();
private:
	std::string file_name;
	WINDOW* file_info_bar;

	void move_cursor(const Cursor& cursor) const noexcept;
	
	void draw_file_info_bar() const noexcept;
};
#endif
