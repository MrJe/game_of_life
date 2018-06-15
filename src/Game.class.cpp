#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <vector>
#include "Game.class.hpp"
#include "nc_window.hpp"

/*
** WARNING: mouse position is unsigned char only... :(
** 222 max
*/

std::string const	Game::_menutxt[MENU_SIZE] = {"X", "O", "Play", "Clean", "Exit"};
std::string const	Game::_pausetxt[PAUSE_SIZE] = {"Resume", "Exit"};

/* ************************************************************************** */
/* ** Coplien's form ******************************************************** */
/* ************************************************************************** */

Game::Game( void ) : _y(0), _x(0), _isplaying(false), _char(1), _cycles(0) {
	getmaxyx(stdscr, this->_height, this->_width);
	this->_new_height = this->_height;
	this->_new_width = this->_width;
	this->_win = create_win(WIN_H, WIN_W, 0, 0);
	this->_menu = create_win(MENU_H, MENU_W, MENU_Y, MENU_X);
	this->_map = new char * [Y_CASE];
	for (int y = 0; y < Y_CASE; ++y) {
		this->_map[y] = new char [X_CASE];
		for (int x = 0; x < X_CASE; ++x)
			this->_map[y][x] = 0;
	}
	return;
}

Game::~Game( void ) {
	destroy_win(this->_win);
	destroy_win(this->_menu);
	for (int y = 0; y < Y_CASE; ++y)
		delete this->_map[y];
	delete [] this->_map;
	return;
}

/* ************************************************************************** */
/* ** Operators ************************************************************* */
/* ************************************************************************** */

std::ostream &		operator << ( std::ostream & o, Game const & rhs) {
	o <<  rhs.getHeight() << "," << rhs.getWidth() << std::endl;
	return (o);
}

/* ************************************************************************** */
/* ** Get & Set ************************************************************* */
/* ************************************************************************** */

WINDOW *	Game::getWin   ( void ) const { return (this->_win); }
int			Game::getHeight( void ) const { return (this->_height); }
int			Game::getWidth ( void ) const { return (this->_width); }
int			Game::getY     ( void ) const { return (this->_y); }
int			Game::getX     ( void ) const { return (this->_x); }

/* ************************************************************************** */
/* ** Functions ************************************************************* */
/* ************************************************************************** */

void		Game::_window_resize( void ) {
	if (resize_win(this->_win, this->_new_height, this->_new_width) == true) {
		char	**tmp;

		tmp = new char * [Y_CASE_NEW];
		for (int y = 0; y < Y_CASE_NEW; ++y) {
			tmp[y] = new char [X_CASE_NEW];
			for (int x = 0; x < X_CASE_NEW; ++x) {
				if (y < Y_CASE && x < X_CASE)
					tmp[y][x] = this->_map[y][x];
				else
					tmp[y][x] = 0;
			}
		}
		for (int y = 0; y < Y_CASE; ++y)
			delete this->_map[y];
		delete [] this->_map;
		this->_map = tmp;
		this->_height = this->_new_height;
		this->_width = this->_new_width;
		wresize(this->_win, WIN_H, WIN_W);
		wresize(this->_menu, MENU_H, MENU_W);
		mvwin(this->_menu, MENU_Y, MENU_X);
		this->_clean_all();
		drawBox(this->_win);
		drawBox(this->_menu);
		this->_draw_grid();
		this->_display();
	}
	return;
}

void		Game::_clean_all( void ) const {
	wclear(stdscr);
	wclear(this->_win);
	wclear(this->_menu);
}

void		Game::_draw_grid( void ) const {
	for (int j = 1; j < WIN_H - 1; j += 1) {
		for (int i = 2; i < WIN_W - 1; i += 2) {
			mvwprintw(this->_win, j, i, " ");
		}
	}
	return;
}

void		Game::_display_selection( void ) const {
	int size = (this->_isplaying) ? PAUSE_SIZE : MENU_SIZE;
	int index = (this->_isplaying) ? this->_pauseindex : this->_menuindex;

	for (int i = 0; i < size; ++i) {
		wattroff(this->_menu, A_STANDOUT);
		if (i == index)
			wattron(this->_menu, A_STANDOUT);
		mvwprintw(this->_menu, 3 + i, 3, "%-9s", Game::_menutxt[i].c_str());
	}
	wattroff(this->_menu, A_STANDOUT);
}

void		Game::_display( void ) const {
	mvwprintw(this->_menu, 1, 2, "GameOfLife");
	for (int y = 0; y < Y_CASE; ++y) {
		for (int x = 0; x < X_CASE; ++x) {
			int				c = this->_map[y][x];
			std::string		toprint;
			if (c == 0)
				toprint = " ";
			else if (c == 1)
				toprint = "X";
			else
				toprint = "O";
			mvwprintw(this->_win, y + 1, x * 2 + 1, toprint.c_str());
		}
	}
	mvwprintw(this->_menu, 10, 3, "Cycles");
	mvwprintw(this->_menu, 11, 3, "%-9d", this->_cycles);
	this->_display_selection();
	wrefresh(this->_win);
	wrefresh(this->_menu);
	return;
}

void		Game::_update_map( int y, int x ) {
	--y;
	x = (x - 1) / 2;
	if (y >= 0 && x >= 0 && y < Y_CASE && x < X_CASE)
		this->_map[y][x] = (this->_map[y][x] == this->_char) ? 0 : this->_char;
	return;
}

void		Game::_change_index( int increment ) {
	int		size = MENU_SIZE;
	int		&index = this->_menuindex;

	if (increment > 0)
		index = (index == size - 1) ? 0 : index + 1;
	else
		index = (index == 0) ? size - 1 : index - 1;
	return;
}

int			Game::_check_neighbors( int y, int x ) const {
	int		alive = 0;
	int		ny;
	int		nx;

	for (int i = -1; i < 2; ++i) {
		ny = i + y;
		if (ny >= 0 && ny < Y_CASE) {
			for (int j = -1; j < 2; ++j) {
				nx = j + x;
				if (nx >= 0 && nx < X_CASE && (ny != y || nx != x)) {
					if (this->_map[ny][nx] > 0)
						++alive;
				}
			}
		}
	}
	//if (x >= 0 && y >= 0 && x <= 1 && y <= 2)
	//	mvwprintw(this->_menu, 10 + y, 3 + (x * 2), "%-9d", alive);
	return (alive);
}

void		Game::_play( void ) {
	int		nb_alive;

	for (int y = 0; y < Y_CASE; ++y) {
		for (int x = 0; x < X_CASE; ++x) {
			char &		cel = this->_map[y][x];

			nb_alive = this->_check_neighbors(y, x);
			if (cel > 0 && (nb_alive < 2 || nb_alive > 3))
				cel = ATOD;
			else if (cel == 0 && nb_alive == 3)
				cel = DTOA;
		}
	}
	for (int y = 0; y < Y_CASE; ++y) {
		for (int x = 0; x < X_CASE; ++x) {
			if (this->_map[y][x] == ATOD)
				this->_map[y][x] = 0;
			else if (this->_map[y][x] == DTOA)
				this->_map[y][x] = 1;
		}
	}
	++(this->_cycles);
	return;
}

void		Game::_clean_cells( void ) {
	for (int y = 0; y < Y_CASE; ++y) {
		for (int x = 0; x < X_CASE; ++x) {
			this->_map[y][x] = 0;
		}
	}
	this->_cycles = 0;
	return;
}

int			Game::load_game( void ) {
	int		ch;
	int		choice = 1;
	MEVENT	event;

	keypad(this->_menu, TRUE);
	this->_menuindex = 0;
	this->_draw_grid();
	this->_display();

	while (choice) {
		this->_window_resize();
		ch = wgetch(this->_menu);

		if (ch == ENTER) {
			std::string		action = Game::_menutxt[this->_menuindex];
			if (action == "Exit")
				choice = 0;
			else if (action == "Clean")
				this->_clean_cells();
			else if (action == "Play")
				this->_play();
			else if (action == "X")
				this->_char = 1;
			else if (action == "O")
				this->_char = 2;
		}
		else if (ch == KEY_UP)
			this->_change_index(-1);
		else if (ch == KEY_DOWN)
			this->_change_index(1);

		if (ch == KEY_MOUSE) {
			if(getmouse(&event) == OK) {
				if(BUTTON1_PRESSED)
					this->_update_map(event.y, event.x);
			}
		}
		this->_display();
	}
	this->_clean_all();
	return (0);
}
