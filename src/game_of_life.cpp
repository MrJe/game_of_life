#include "nc_window.hpp"
#include "Game.class.hpp"

void		game_of_life( void ) {
	Game	game;

	while (game.load_game());
	return;
}

int			main( int argc, char const * argv[] ) {
	static_cast<void>(argv);
	if (argc == 1) {
		init_ncurses();
		game_of_life();
		terminate_ncurses(0);
	}
	else
		std::cout << "usage: ./game_of_life" << std::endl;
	return (0);
}
