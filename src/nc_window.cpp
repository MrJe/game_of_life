#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

void		init_ncurses( void ) {
	initscr();				// Initialise la structure WINDOW et autres paramètres
	start_color();			// Start color
	raw();					// Line buffering disabled / cbreak() ?
	keypad(stdscr, TRUE);	// Get F1, arrows, etc.
	noecho();				// Don't echo() while we do getch
	curs_set(FALSE);		// Hide the cursor
	nodelay(stdscr, TRUE);	// getchar() don't wait ? TODO: check that
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL); // active mouse
}

WINDOW *	create_win(int height, int width, int y, int x) {
	WINDOW *	win = NULL;

	win = newwin(height, width, y, x);
	box(win, ACS_VLINE, ACS_HLINE);
	wrefresh(win);
	return (win);
}

void		destroy_win(WINDOW *win) {
	wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(win);
	delwin(win);
}

void		drawBoxCh( WINDOW * win, char c1, char c2) {
	box(win, c1, c2);
	touchwin(win); // TODO: check that
	wrefresh(win);
}

//wborder(win, '[', ']', '^', 'v', '0', '1', '2', '3');
void		drawBox( WINDOW * win ) {
	box(win, ACS_VLINE, ACS_HLINE);
	touchwin(win);
	wrefresh(win);
}


void		terminate_ncurses( int ret ) {
	endwin();
	if (ret)
		exit(ret);
}

bool		resize_win( WINDOW * win, int & height, int & width ) {
	int		new_height, new_width;

	getmaxyx(stdscr, new_height, new_width);
	if (new_height != height || new_width != width) {
		height = new_height;
		width = new_width;
		wclear(win); // like werase(), but also call clearok()
		return (true);
	}
	return (false);
}
