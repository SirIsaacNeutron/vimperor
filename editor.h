#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include <vector>
#include <string>

#include "screen.h"
#include "cursor.h"

class Editor {
	FILE* file;
	std::vector<std::string> file_contents;
	Screen screen;
	Cursor cursor;

	std::vector<std::string> create_file_contents() noexcept;
public:
	Editor(const char* file_name = "") noexcept;
	Editor(const Editor& e) = delete;
	Editor& operator=(const Editor& e) = delete;

	void process_keypress(int character) noexcept;
	
	~Editor();
};

#endif
