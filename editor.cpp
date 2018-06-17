#include "editor.h"

Editor::Editor(const char* file_name) noexcept
: file{fopen(file_name, "r+")}, file_contents{create_file_contents()},
screen{}, cursor{0, 0}, file_contents_index{0}, top_of_screen_index{0} {
	screen.show_first_display(std::begin(file_contents));
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
			++cursor.x;
			screen.move_cursor(cursor);
			break;
		case 'k':
			move_cursor_up();
			break;
		case 'j':
			move_cursor_down();
			break;
		
		case 'h':
			--cursor.x;
			screen.move_cursor(cursor);
			break;
	}
}

void Editor::move_cursor_up() noexcept {
	if (cursor.y != 0) {
		--cursor.y;
		--file_contents_index;
	}
	else if (file_contents_index != 0 && top_of_screen_index != 0) {
		--file_contents_index; 
		--top_of_screen_index;
		screen.display(std::begin(file_contents)
				+ top_of_screen_index);		
	}
	screen.move_cursor(cursor);
}

void Editor::move_cursor_down() noexcept {
	if (cursor.y != screen.rows) {
		++cursor.y;	
		++file_contents_index;
	}
	else if (file_contents_index < file_contents.size() && 
		top_of_screen_index < file_contents.size()) {
			++top_of_screen_index;
			++file_contents_index;
			screen.display(std::begin(file_contents)
					+ top_of_screen_index);	
	}
	screen.move_cursor(cursor);
}

Editor::~Editor() {
	fclose(file);
}
