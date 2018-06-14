#ifndef NC_WINDOW_HPP
# define NC_WINDOW_HPP

# include <ncurses.h>

void		init_ncurses( void );
void		terminate_ncurses( int ret );

WINDOW *	create_win( int height, int width, int y, int x );
void		destroy_win( WINDOW * win );

void		drawBoxCh( WINDOW * win, char c1, char c2);
void		drawBox( WINDOW * win );

bool		resize_win( WINDOW * win, int & height, int & width );

#endif
