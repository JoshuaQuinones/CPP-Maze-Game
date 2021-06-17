#include "MazeGame.h"
/* 'main' function used to create a maze game and start playing it */

bool askToPlayAgain(); //prototype

int main()
{
	MazeGame myMazeGame;
	myMazeGame.playGame();
	//take user input to see if they want to play again
	while (askToPlayAgain())
	{
		myMazeGame.resetMaze();
		myMazeGame.playGame();
	}

	return 0;
}

/*Function for main to ask the player if they would like to play again*/
bool askToPlayAgain()
{
	char userInput;
	bool validInput = false;
	while (!validInput)
	{
		std::cout << "\nWould you like to play again? (Y/N)\n";
		std::cin >> userInput;
		if (std::cin.fail())
		{
			std::cout << "Invalid input.\n";
			std::cin.clear(); //clear error state
			std::cin.ignore(INT_MAX); //ignore all additional input until eof
		}
		else if (userInput == 'Y' || userInput == 'y')
		{
			return true;
			validInput = true;
		}
		else if (userInput == 'N' || userInput == 'n')
		{
			return false;
			validInput = true;
		}
		else
		{
			std::cout << "Invalid input.\n";
		}
	}
	return false; //shouldn't reach this point
}