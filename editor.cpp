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
	switch (character) {
		case 'q':
			endwin();
			exit(1);
			break;
		case 'l':
			if (cursor.x < screen.cols) {
				++cursor.x;
			}
			screen.move_cursor(cursor);
			break;
		case 'k':
			move_cursor_up();
			break;
		case 'j':
			move_cursor_down();
			break;
		
		case 'h':
			if (cursor.x > 0) {
				--cursor.x;
			}
			screen.move_cursor(cursor);
			break;
		case 's':
			save();
			break;
		default:
			if ((character >= 'a' && character <= 'z')
				|| (character >= 'A' && character <= 'Z')
				|| character == ' ') {
				write_char(character);
			}
	}
}

void Editor::move_cursor_up() noexcept {
	if (cursor.y != 0) {
		--cursor.y;
		--file_contents_index;
	}
	else if (file_contents_index != 0) {
		--file_contents_index; 
		--top_of_screen_index;
		screen.display(std::begin(file_contents)
				+ top_of_screen_index, std::end(file_contents));		
	}
	screen.move_cursor(cursor);
}

void Editor::move_cursor_down() noexcept {
	if (cursor.y != screen.rows) {
		++cursor.y;	
		++file_contents_index;
	}
	else if (file_contents_index < file_contents.size()) {
			++top_of_screen_index;
			++file_contents_index;
			screen.display(std::begin(file_contents)
					+ top_of_screen_index, std::end(file_contents));	
	}
	screen.move_cursor(cursor);
}

void Editor::write_char(int character) noexcept {	
	if (cursor.x < file_contents[cursor.y].size()) {
		file_contents[cursor.y][cursor.x] = character;
		++cursor.x;
	}
	else {
		file_contents[cursor.y].push_back(character);
		++cursor.x;
	}
	screen.move_cursor(cursor);
	screen.display(std::begin(file_contents)
			+ top_of_screen_index, std::end(file_contents));	
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
