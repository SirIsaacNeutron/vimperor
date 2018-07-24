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
	if (current_mode == Mode::NORMAL) {
		normal_mode_action(character);
	}
	else if (current_mode == Mode::INSERT) {
		insert_mode_action(character);	
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
		case 's':
			save();
			break;
	}
}

void Editor::move_cursor_right() noexcept {
	if (file_contents_index < file_contents.size()
			&& cursor.x < file_contents[file_contents_index].size()) {
		if (file_contents[file_contents_index][cursor.x] == '\t') {
			for (int i = 0; i < SPACES_FOR_TAB; ++i) {
				++cursor.x;	
			}
		}
		else {
			++cursor.x;
		}
		screen.move_cursor(cursor);
	}
}

void Editor::move_cursor_up() noexcept {
	// Logic for whether the cursor should be moved horizontally.
	if (file_contents_index != 0) {
		const auto& line_above = file_contents[file_contents_index - 1];
		const auto& current_line = file_contents[file_contents_index];

		if (line_above.size() < current_line.size()) {
			if (line_above.size() == 0) {
				cursor.x = 0;
			}
			else if (cursor.x > line_above.size()) {
				cursor.x = line_above.size() - 1;
			}
		}
	}

	if (cursor.y != 0) {
		--cursor.y;
		--file_contents_index;
		screen.move_cursor(cursor);
	}
	else if (file_contents_index != 0) {
		--file_contents_index; 
		--top_of_screen_index;
	}
}

void Editor::move_cursor_down() noexcept {
	if (file_contents_index < file_contents.size()) {
		// We check for cursor.y + 1 here rather than just cursor.y.
		// This is because cursor.y + 1 == screen.rows when we've
		// moved the cursor down when it was at the bottom of the screen.
		// (It's not possible to see the cursor at row screen.rows, 
		// because screen.rows is offscreen.)
		if (cursor.y + 1 != screen.rows) {
			++cursor.y;	
			++file_contents_index;
			screen.move_cursor(cursor);
		}
		else {
			++top_of_screen_index;
			++file_contents_index;
		}
	}
}

void Editor::move_cursor_left() noexcept {
	if (cursor.x > 0) {
		--cursor.x;
		screen.move_cursor(cursor);
	}
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
			if (std::isprint(character)) {
				write_char(character);
			}
			break;
	}
}

void Editor::delete_char() noexcept {
	// If we're not past the end of the file
	if (file_contents_index < file_contents.size()) {
		// If a line is empty and we just pressed the delete key at the left-
		// most edge of the screen
		if (cursor.x == 0 
				&& file_contents[file_contents_index].size() == 0) {
			// Then delete the whole line	
			file_contents.erase(std::begin(file_contents) + file_contents_index);
		}
		else {
			file_contents[file_contents_index].replace(cursor.x, 1, "");
		}
		screen.is_file_modified = true;
	}
	move_cursor_left();
}

void Editor::add_new_line() noexcept {
	if (file_contents_index < file_contents.size()) {
		std::string rest_of_line{ 
			std::begin(file_contents[file_contents_index]) + cursor.x,
			std::end(file_contents[file_contents_index])};

		file_contents[file_contents_index].erase(
			cursor.x, file_contents[file_contents_index].size());

		file_contents.insert(
				std::begin(file_contents) + file_contents_index + 1,
				rest_of_line);

		move_cursor_down();
		cursor.x = 0;
	}
	else {
		file_contents.push_back("");
	}

	screen.is_file_modified = true;
}

void Editor::write_char(int character) noexcept {	
	if (file_contents_index < file_contents.size()) {
		if (cursor.x < file_contents[file_contents_index].size()) {
			file_contents[file_contents_index][cursor.x] = character;

			move_cursor_right();
		}
		// If we're at the end of the current line
		else {
			file_contents[file_contents_index].push_back(character);

			move_cursor_right();
		}
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
