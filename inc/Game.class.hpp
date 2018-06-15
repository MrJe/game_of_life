#ifndef GAME_CLASS_HPP
# define GAME_CLASS_HPP

# include <iostream>
# include <string>
# include <ncurses.h>
# include <vector>

# define ESC    27
# define ENTER  10

# define WIN_H  (this->_height)
# define WIN_W  (this->_width - MENU_W)

# define MENU_H (this->_height)
# define MENU_W 15
# define MENU_Y 0
# define MENU_X (WIN_W)

# define Y_CASE (WIN_H - 2)
# define X_CASE ((WIN_W - 1) / 2)
# define Y_CASE_NEW (this->_new_height - 2)
# define X_CASE_NEW ((this->_new_width - MENU_W) / 2)

# define MENU_SIZE 5
# define PAUSE_SIZE 2

# define ATOD 9
# define ATOA 8
# define DTOA -8
# define DTOD -9

class		Game {

/* ************************************************************************** */
/* ** P U B L I C *********************************************************** */
/* ************************************************************************** */
public:
	Game( void );
	virtual ~Game( void );

	int			load_game( void );

	WINDOW *	getWin ( void )   const;
	int			getHeight( void ) const;
	int			getWidth( void )  const;
	int			getY( void )      const;
	int			getX( void )      const;

/* ************************************************************************** */
/* ** P R O T E C T E D ***************************************************** */
/* ************************************************************************** */
protected:

/* ************************************************************************** */
/* ** P R I V A T E ********************************************************* */
/* ************************************************************************** */
private:
	Game( Game const & src );
	Game &		operator = ( Game const & rhs );

	WINDOW *	_win;
	WINDOW *	_menu;
	int			_height;
	int			_width;
	int			_new_height;
	int			_new_width;
	int			_y;
	int			_x;
	bool		_isplaying;
	char		_char;
	int			_cycles;

	int							_menuindex;
	static std::string const	_menutxt[MENU_SIZE];
	int							_pauseindex;
	static std::string const	_pausetxt[PAUSE_SIZE];

	void		_window_resize( void );

	void		_display( void ) const;
	void		_change_index( int increment );
	void		_display_selection( void ) const;
	void		_draw_grid( void ) const;
	void		_clean_all( void ) const;
	void		_update_map( int y, int x );
	void		_play( void );
	int			_check_neighbors( int y, int x ) const;
	void		_clean_cells( void );

	char **		_map;
};

std::ostream &		operator << ( std::ostream & o, Game const & rhs);

#endif
