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
	INSERT,
	REPLACE
};

static const int ESCAPE_KEY = '\x1B';
static const int BACKSPACE_KEY = '\x7F';
static const int SPACES_FOR_TAB = 8;
static const int ENTER_KEY = '\xA';

// The core functionality of Vimperor.
class Editor {
	FILE* file;
	Screen screen;
	Mode current_mode{Mode::NORMAL};
	std::vector<std::string> file_contents{create_file_contents()};
	Cursor cursor{};
	std::size_t file_contents_index = 0;
	std::size_t top_of_screen_index = 0;

	std::vector<std::string> create_file_contents() noexcept;

	void move_cursor_down() noexcept;
	void move_cursor_up() noexcept;
	void move_cursor_left() noexcept;
	void move_cursor_right() noexcept;
	void move_cursor_x_considering_lines(
			const std::string& line_considered,
			const std::string& current_line) noexcept;

	void normal_mode_action(int character) noexcept;
	void insert_mode_action(int character) noexcept;
	void replace_mode_action(int character) noexcept;

	void do_w_motion() noexcept;
	void do_b_motion() noexcept;

	void insert_char(int character) noexcept;
	void replace_char(int character) noexcept;
	void delete_char() noexcept;
	void save() noexcept;
	void add_new_line() noexcept;
public:
	Editor(const char* file_name = "") noexcept;
	Editor(const Editor& e) = delete;
	Editor& operator=(const Editor& e) = delete;

	void process_keypress(int character) noexcept;
	
	~Editor();
};

#endif
