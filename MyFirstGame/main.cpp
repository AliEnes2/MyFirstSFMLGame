#include <iostream>

#include "Game.h"


int main()
{
	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	//Init Game Engine 
	Game game;
	

	//Game Loop
	while (game.running() && !game.getEndGame())
	{
		//Update
		game.update();

		//Render
		game.render();

	}


	//End
	return 0;
}