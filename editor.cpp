#include "editor.h"

Editor::Editor(const char* file_name) noexcept
: file{fopen(file_name, "r+")}, file_contents{create_file_contents()},
screen{}, cursor{0, 0} {
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
			--cursor.y;
			screen.move_cursor(cursor);
			break;
		case 'j':
			++cursor.y;	
			screen.move_cursor(cursor);
			break;
		case 'h':
			--cursor.x;
			screen.move_cursor(cursor);
			break;
	}
}

Editor::~Editor() {
	fclose(file);
}
