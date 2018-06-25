#include "editor.h"

int main() {
	Editor e{"editor.cpp"};	

	int character;
	while (true) {
		character = getch();
		e.process_keypress(character);
	}
	return 0;
}
