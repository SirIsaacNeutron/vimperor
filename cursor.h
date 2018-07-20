#ifndef CURSOR_H
#define CURSOR_H

// A simple struct representing the coordinates of the cursor on the screen.
// Keep in mind that Cursor does not keep track of where the user is in
// the file; that's the job of the Editor class.
struct Cursor {
	unsigned int x;
	unsigned int y;
};
#endif
