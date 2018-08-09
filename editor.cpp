#include "editor.h"

Editor::Editor(const char* file_name) noexcept
: file{fopen(file_name, "r+")}, screen{file_name} {
	screen.display(std::begin(file_contents), 
			std::end(file_contents),
			cursor);
}

std::vector<std::string> Editor::create_file_contents() noexcept {
	std::vector<std::string> file_contents;
	std::string file_line;
	char c;
	while ((c = fgetc(file)) != EOF) {
		if (c != '\n') {
			file_line += c;	
		}
		else {
			file_contents.push_back(file_line);
			file_line = "";
		}
	}
	return file_contents;
}

void Editor::process_keypress(int character) noexcept {
	switch (current_mode) {
		case Mode::NORMAL: 
			normal_mode_action(character);
			break;
		case Mode::INSERT:
			insert_mode_action(character);	
			break;
		case Mode::REPLACE:
			replace_mode_action(character);
			break;
	}
	screen.display(std::begin(file_contents) + top_of_screen_index,
			std::end(file_contents),
			cursor);
}

void Editor::normal_mode_action(int character) noexcept {
	switch (character) {
		case 'i':
			current_mode = Mode::INSERT;
			break;
		case 'q':
			endwin();
			exit(1);
			break;
		case 'l':
			move_cursor_right();
			break;
		case 'k':
			move_cursor_up();
			break;
		case 'j':
			move_cursor_down();
			break;
		case 'h':
			move_cursor_left();
			break;
		case 'x':
			file_contents[file_contents_index].replace(cursor.row_offset, 1, "");
			break;
		case 'w':
			do_w_motion();
			break;
		case 'b':
			do_b_motion();
			break;
		case 'R':
			current_mode = Mode::REPLACE;
			break;
		case 's':
			save();
			break;
	}
}

void Editor::move_cursor_right() noexcept {
	const auto& current_line = file_contents[file_contents_index];
	if (file_contents_index < file_contents.size()
			&& cursor.row_offset < current_line.size()) {
		if (current_line[cursor.row_offset] == '\t') {
			cursor.x += SPACES_FOR_TAB;
			++cursor.row_offset;
		}
		else {
			++cursor.x;
			++cursor.row_offset;
		}
	}
}

void Editor::move_cursor_up() noexcept {
	if (file_contents_index != 0) {
		const auto& line_above = file_contents[file_contents_index - 1];
		const auto& current_line = file_contents[file_contents_index];

		move_cursor_x_considering_lines(line_above, current_line);
	}

	if (cursor.y != 0) {
		--cursor.y;
		--file_contents_index;
	}
	else if (file_contents_index != 0) {
		--file_contents_index; 
		--top_of_screen_index;
	}
}

void Editor::move_cursor_x_considering_lines(
const std::string& line_considered, const std::string& current_line) noexcept {
	if (line_considered.size() < current_line.size()) {
		if (line_considered.size() == 0) {
			cursor.x = 0;
			cursor.row_offset = 0;
		}
		else if (cursor.x > line_considered.size()) {
			cursor.x = line_considered.size() - 1;
			cursor.row_offset = cursor.x;
		}
	}
}

void Editor::move_cursor_down() noexcept {
	if (file_contents_index < file_contents.size()) {
		if (file_contents_index + 1 < file_contents.size()) {
			const auto& current_line = file_contents[file_contents_index];
			const auto& line_below = file_contents[file_contents_index + 1];
			move_cursor_x_considering_lines(line_below, current_line);

		}
		// We check for cursor.y + 1 here rather than just cursor.y.
		// This is because cursor.y + 1 == screen.rows when we've
		// moved the cursor down when it was at the bottom of the screen.
		// (It's not possible to see the cursor at row screen.rows, 
		// because screen.rows is offscreen.)
		if (cursor.y + 1 != screen.rows) {
			++cursor.y;	
			++file_contents_index;
		}
		else {
			++top_of_screen_index;
			++file_contents_index;
		}
	}
}

void Editor::move_cursor_left() noexcept {
	if (cursor.row_offset > 0) {
		const auto& current_line = file_contents[file_contents_index];
		if (current_line[cursor.row_offset - 1] == '\t') {
			cursor.x -= SPACES_FOR_TAB;
			--cursor.row_offset;
		}
		else {
			--cursor.x;
			--cursor.row_offset;
		}
	}
}

void Editor::do_w_motion() noexcept {
	std::size_t next_space_index = file_contents[file_contents_index]
		.find_first_of(" ", cursor.x);

	if (next_space_index != std::string::npos) {
		cursor.x = next_space_index + 1;	
		cursor.row_offset = cursor.x;
	}
}

void Editor::do_b_motion() noexcept {
	const auto& current_line = file_contents[file_contents_index];

	std::size_t previous_space_index = cursor.x;
	for (; previous_space_index != 0; --previous_space_index) {
		if (current_line[previous_space_index] == ' ') {
			cursor.x = previous_space_index - 1;	
			break;
		}
	}

	if (previous_space_index == 0) {
		cursor.x = 0;
	}
	cursor.row_offset = cursor.x;
}

void Editor::insert_mode_action(int character) noexcept {
	switch (character) {
		case ESCAPE_KEY:
			current_mode = Mode::NORMAL;
			break;
		case BACKSPACE_KEY:
			delete_char();
			break;
		case ENTER_KEY:
			add_new_line();
			break;
		default:
			if (std::isprint(character) || character == '\t') {
				insert_char(character);
			}
			break;
	}
}

void Editor::replace_mode_action(int character) noexcept {
	if (character == ESCAPE_KEY) {
		current_mode = Mode::NORMAL;
		return;
	}
	else if (std::isprint(character)) {
		replace_char(character);
	}
}

void Editor::delete_char() noexcept {
	// If we're not past the end of the file
	if (file_contents_index < file_contents.size()) {
		// If a line is empty and we just pressed the delete key at the left-
		// most edge of the screen
		auto& current_line = file_contents[file_contents_index];
		if (cursor.row_offset == 0 
				&& current_line.size() == 0) {
			// Then delete the whole line	
			file_contents.erase(std::begin(file_contents) + file_contents_index);
		}
		else {
			current_line.replace(cursor.row_offset, 1, "");
		}
		screen.is_file_modified = true;
	}
	move_cursor_left();
}

void Editor::add_new_line() noexcept {
	if (file_contents_index < file_contents.size()) {
		auto& current_line = file_contents[file_contents_index];

		std::string rest_of_line{std::begin(current_line) + cursor.row_offset,
			std::end(current_line)};

		current_line.erase(cursor.row_offset, current_line.size());

		file_contents.insert(
				std::begin(file_contents) + file_contents_index + 1,
				rest_of_line);

		move_cursor_down();
		cursor.x = 0;
		cursor.row_offset = 0;
	}
	else {
		file_contents.push_back("");
	}

	screen.is_file_modified = true;
}

void Editor::insert_char(int character) noexcept {
	if (file_contents_index < file_contents.size()) {
		auto& current_line = file_contents[file_contents_index];
		if (cursor.row_offset < file_contents[file_contents_index].size()) {
			current_line.insert(cursor.row_offset, 1, character);

		}
		// If we're at the end of the current line
		else {
			current_line.push_back(character);

		}
		move_cursor_right();
	}
	// If we're beyond the end of the file
	else {
		// Append a new, empty line
		file_contents.push_back("");
	}
	screen.is_file_modified = true;
}

void Editor::replace_char(int character) noexcept {	
	if (file_contents_index < file_contents.size()) {
		auto& current_line = file_contents[file_contents_index];
		if (cursor.row_offset < file_contents[file_contents_index].size()) {
			current_line[cursor.row_offset] = character;
		}
		// If we're at the end of the current line
		else {
			current_line.push_back(character);
		}
		move_cursor_right();
	}
	// If we're beyond the end of the file
	else {
		// Append a new, empty line
		file_contents.push_back("");
	}
	screen.is_file_modified = true;
}

void Editor::save() noexcept {
	rewind(file);
	for (const auto& str : file_contents) {
		fputs(str.c_str(), file);	
		fputs("\n", file);
	}
	screen.is_file_modified = false;
}

Editor::~Editor() {
	fclose(file);
}
