#include "editor.h"

int main() {
	Editor e{"makefile"};	

	int character;
	while (true) {
		character = getch();
		e.process_keypress(character);
	}
	return 0;
}
