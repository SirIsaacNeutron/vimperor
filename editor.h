#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>

class Editor {
	FILE* file;
public:
	Editor(const char* file_name = "");
	
	~Editor();
};

#endif
