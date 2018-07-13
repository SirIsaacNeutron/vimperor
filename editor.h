#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include <vector>
#include <string>
#include <cctype>

#include "screen.h"
#include "cursor.h"

enum class Mode {
	NORMAL = 0,
	INSERT
};

static const int ESCAPE_KEY = '\x1B';

class Editor {
	FILE* file;
	Mode current_mode{Mode::NORMAL};
	std::vector<std::string> file_contents{create_file_contents()};
	Screen screen{};
	Cursor cursor{0, 0};
	std::size_t file_contents_index = 0;
	std::size_t top_of_screen_index = 0;

	std::vector<std::string> create_file_contents() noexcept;
	void move_cursor_down() noexcept;
	void move_cursor_up() noexcept;
	void move_cursor_left() noexcept;
	void move_cursor_right() noexcept;

	void normal_mode_action(int character) noexcept;
	void insert_mode_action(int character) noexcept;
	void write_char(int character) noexcept;
	void save() noexcept;
public:
	Editor(const char* file_name = "") noexcept;
	Editor(const Editor& e) = delete;
	Editor& operator=(const Editor& e) = delete;

	void process_keypress(int character) noexcept;
	
	~Editor();
};

#endif
