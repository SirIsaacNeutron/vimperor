#include "editor.h"

void run_editor(const char* file_name) {
	Editor e{file_name};	

	int character;
	while (true) {
		character = getch();
		e.process_keypress(character);
	}
}

int main(int argc, char** argv) {
	if (argc > 0) {
		run_editor(argv[1]);
	}
}
