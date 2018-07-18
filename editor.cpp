#include "editor.h"

Editor::Editor(const char* file_name) noexcept
: file{fopen(file_name, "r+")} {
	screen.show_first_display(std::begin(file_contents), 
			std::end(file_contents));
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
	if (cursor.y != 0) {
		--cursor.y;
		--file_contents_index;
		screen.move_cursor(cursor);
	}
	else if (file_contents_index != 0) {
		--file_contents_index; 
		--top_of_screen_index;
		screen.display(std::begin(file_contents)
				+ top_of_screen_index, std::end(file_contents));		
	}
}

void Editor::move_cursor_down() noexcept {
	if (file_contents_index < file_contents.size()) {
		if (cursor.y + 1 != screen.rows) {
			++cursor.y;	
			++file_contents_index;
			screen.move_cursor(cursor);
		}
		else {
			++top_of_screen_index;
			++file_contents_index;
			screen.display(std::begin(file_contents)
					+ top_of_screen_index, std::end(file_contents));	
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
		default:
			if (std::isprint(character)) {
				write_char(character);
			}
			break;
	}
}

void Editor::write_char(int character) noexcept {	
	if (file_contents_index < file_contents.size()) {
		if (cursor.x < file_contents[file_contents_index].size()) {
			file_contents[file_contents_index][cursor.x] = character;
			move_cursor_right();

			screen.move_cursor(cursor);
			screen.display(std::begin(file_contents)
				+ top_of_screen_index, std::end(file_contents));
		}
		else {
			file_contents[file_contents_index].push_back(character);
			move_cursor_right();

			screen.move_cursor(cursor);
			screen.display(std::begin(file_contents)
				+ top_of_screen_index, std::end(file_contents));
		}
	}
	else {
		file_contents.push_back("");
	}
}

void Editor::save() noexcept {
	rewind(file);
	for (const auto& str : file_contents) {
		fputs(str.c_str(), file);	
		fputs("\n", file);
	}
}

Editor::~Editor() {
	fclose(file);
}
