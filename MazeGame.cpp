#include "MazeGame.h"
/*Implement the methods of the MazeGame class, which is used to play a maze game in the console*/

//MazeGame constructor. Calls the resetMaze() method to get info for the first maze to play
MazeGame::MazeGame()
{
	this->resetMaze();
}

/*Takes user input to read a maze from a file, store the maze in a 2D vector, and save the indices of the player, wanderer, seeker, and goal*/
void MazeGame::resetMaze()
{
	this->mazeState.clear(); //make sure maze is empty
	this->wanderStandingOver = '-'; //Wanderer's starting position will be over a '-' space by default
	//ask user for the filename and attempt to open it
	std::string filename;
	std::cout << "Enter the maze's filename:\n";
	std::cin >> filename;
	std::ifstream mazeFile;
	mazeFile.open(filename);
	//if the file could not be opened, ask the user for another filename and try again until the file opens
	while (mazeFile.fail())
	{
		std::cout << "Could not open the file\nEnter the maze's filename:\n";
		std::cin >> filename;
		mazeFile.open(filename);
	}
	//Assume the maze file is formatted correctly. The first line of a maze file contains three ints: the first two representing the size of the maze, and the third being the starting score for that maze
	int sizeX, sizeY;
	mazeFile >> sizeX;
	mazeFile >> sizeY;
	mazeFile >> this->playerScore; //set the player's initial score
	if (mazeFile.fail())
	{
		throw std::runtime_error("Could not read maze size from file");
	}
	//after reading the 3 ints, read the contents of the maze itself, char by char, and store them in the 2D vector mazeState. Also, look for the player char 'P' and store its index when found
	std::vector<char> currentRow; //store characters of the current row, and will push_back to mazeState when filled
	char currentChar;
	//use nested for loop to keep track of current index when adding characters to the current row, to store indices of importent chars and know when to move to the next row when the current one is filled
	//REMINDER: In 2D vector, access a variable with vectorName[Ycoordinate][Xcoordinate]
	for (int i = 0; i < sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			mazeFile >> currentChar;
			if (currentChar == 'P')
			{
				this->playerIndex = std::pair<int, int>(i, j);
			}
			else if (currentChar == 'G')
			{
				this->goalIndex = std::pair<int, int>(i, j);
			}
			else if (currentChar == 'W')
			{
				this->wanderIndex = std::pair<int, int>(i, j);
			}
			else if (currentChar == 'S')
			{
				this->seekerIndex = std::pair<int, int>(i, j);
			}
			currentRow.push_back(currentChar);
		}
		this->mazeState.push_back(currentRow);
		currentRow.clear();
	}
}

//Print the current state of the maze to the console
void MazeGame::displayBoard()
{
	for (int i = 0; i < this->mazeState.size(); i++)
	{
		for (int j = 0; j < this->mazeState[i].size(); j++)
		{
			std::cout << this->mazeState[i][j] << "  ";
		}
		std::cout << "\n"; //end of this row, go to next line
	}
}

//Play the game. Continuously take turns from the player while keeping track of the game state and the player's score until the game ends. The game ends when the player quits, reaches the objective, or is caught by an enemy.
void MazeGame::playGame()
{
	//display initial maze state and player information, along with instructions
	std::cout << "\nThe player must navigate their character 'P' to the end goal 'G' to win the game.\nBarriers are shown with 'Z' and cannot be moved to.\n"
		<< "Treasure spaces '+' can be moved to and increase score by 1. Empty spaces '-' can be moved to and decrease score by 1.\n"
		<< "The Seeker, 'S', will chase the player through the maze. The Wanderer, 'W', moves randomly. \nIf either catches the player, the game ends.\n"
		<< "SCORE: " << this->playerScore << "\n";
	this->displayBoard();
	SeekerAI seeker(this->seekerIndex, &this->mazeState); //use SeekerAI class to determine seeker's next move
	while (makePlayerTurn()) //continue making turns until the function returns false (meaning the game has ended)
	{
		//Have the seeker determine and make its move
		this->seekerIndex = seeker.makeTurn(this->playerIndex, this->wanderIndex, this->goalIndex);
		//Check if the seeker has caught the player
		if (this->seekerIndex == std::pair<int, int>(-1, -1))
		{
			this->gameOverMessage = "The seeker has caught the player! GAME OVER.\nSCORE: " + std::to_string(this->playerScore);
			break;
		}
		//Have the wanderer randomly select and then make its move. Break the loop if the player has been caught
		if (this->makeWanderTurn())
		{
			this->gameOverMessage = "The wanderer has caught the player! GAME OVER.\nSCORE: " + std::to_string(this->playerScore);
			break;
		}
		std::cout << "SCORE: " << this->playerScore << "\n";
		this->displayBoard();
	}
	//Now that the game has ended, print the final state of the maze and display the gameOverMessage
	this->displayBoard();
	std::cout << "\n" << this->gameOverMessage;
}

/*Randomly chose from any possible turn the wanderer can make, and then perform it. Return true if the wanderer catches the player*/
bool MazeGame::makeWanderTurn()
{
	std::vector<char> possibleTurns;
	//check N, S, E, and W and see if they can be moved to (are not boundaries, past the edges of the maze, or the goal).
	//check if north possible
	if (this->wanderIndex.first != 0 //out of bounds
		&& this->mazeState[this->wanderIndex.first - 1][this->wanderIndex.second] != 'Z' //into a barrier
		&& this->mazeState[this->wanderIndex.first - 1][this->wanderIndex.second] != 'S') //into the seeker
	{
		possibleTurns.push_back('N');
	}
	//check if south possible
	if (this->wanderIndex.first != this->mazeState.size() - 1 //out of bounds
		&& this->mazeState[this->wanderIndex.first + 1][this->wanderIndex.second] != 'Z' //into a barrier
		&& this->mazeState[this->wanderIndex.first + 1][this->wanderIndex.second] != 'S') //into the seeker
	{
		possibleTurns.push_back('S');
	}
	//check if east possible
	if (this->wanderIndex.second != this->mazeState.at(0).size() - 1 //out of bounds
		&& this->mazeState[this->wanderIndex.first][this->wanderIndex.second + 1] != 'Z' //into a barrier
		&& this->mazeState[this->wanderIndex.first][this->wanderIndex.second + 1] != 'S') //into the seeker
	{
		possibleTurns.push_back('E');
	}
	//check if west possible
	if (this->wanderIndex.second != 0 //out of bounds
		&& this->mazeState[this->wanderIndex.first][this->wanderIndex.second - 1] != 'Z' //into a barrier
		&& this->mazeState[this->wanderIndex.first][this->wanderIndex.second - 1] != 'S') //into the seeker
	{
		possibleTurns.push_back('W');
	}
	//see if any moves are possible
	if (possibleTurns.size() == 0)
	{
		std::cout << "WANDERER'S TURN: Wanderer cannot do anything and will skip its turn\n";
		return false;
	}
	else
	{
		//use random number generator to pick which possible move to make
		char wanderTurn;
		wanderTurn = possibleTurns[std::rand() % possibleTurns.size()];
		//make the turn, updating the current wanderer location and its next location
		this->mazeState[this->wanderIndex.first][this->wanderIndex.second] = this->wanderStandingOver; //set current wanderlocation back to its original value
		switch (wanderTurn) //update wanderer's position
		{
			case 'N':
				std::cout << "WANDERER'S TURN: The wanderer is moving north\n";
				this->wanderIndex.first -= 1;
				break;
			case 'S':
				std::cout << "WANDERER'S TURN: The wanderer is moving south\n";
				this->wanderIndex.first += 1;
				break;
			case 'E':
				std::cout << "WANDERER'S TURN: The wanderer is moving east\n";
				this->wanderIndex.second += 1;
				break;
			case 'W':
				std::cout << "WANDERER'S TURN: The wanderer is moving west\n";
				this->wanderIndex.second -= 1;
				break;
			default:
				throw "Error with wanderer selecting its turn";
		}
		//see if the Wanderer has caught the player
		if (this->mazeState[this->wanderIndex.first][this->wanderIndex.second] == 'P')
		{
			this->mazeState[this->wanderIndex.first][this->wanderIndex.second] = 'W';
			return true;
		}
		this->wanderStandingOver = this->mazeState[this->wanderIndex.first][this->wanderIndex.second];
		this->mazeState[this->wanderIndex.first][this->wanderIndex.second] = 'W';
		return false;
	}
}

/*Take in user input for their turn and modify the maze and their score accordingly. The player can move in 4 cardinal directions (N, S, E, W), chose to stay in place, or can quit the game.
  If the player's turn cannot be made (ex: moving past the boundaries of the maze, moving into a barrier, etc.), then ask the user for input again. Return false if the player choses to quit*/
bool MazeGame::makePlayerTurn()
{
	char playerInput;
	while (true) //will repeat the switch statement in some cases. When a valid move is made, will return and exit the while loop
	{
		std::cout << "\nEnter N,S,E, or W to go North, South, East, or West\nEnter O to stay in place and skip your turn\nEnter Q to quit your turn\n";
		std::cin >> playerInput;
		playerInput = toupper(playerInput); //so user input isn't case sensitive
		switch (playerInput)
		{
		//N,S,E, and W will check the same things, but at different indices. Change index to check depending on the case
		case 'N': 
			//check if move is valid/can be made (isn't into a barrier, enemy, or out of bounds). 
			if (this->playerIndex.first == 0 //out of bounds
				|| this->mazeState[this->playerIndex.first - 1][this->playerIndex.second] == 'Z' //into a barrier
				|| this->mazeState[this->playerIndex.first - 1][this->playerIndex.second] == 'W' //into the wanderer
				|| this->mazeState[this->playerIndex.first - 1][this->playerIndex.second] == 'S') //into the seeker
			{
				std::cout << "This move is invalid. Enter a different move.\n";
				break;
			}
			return this->makePlayerTurnHelper(-1, 0); //1 space north would be the first index - 1, the same second index
		case 'S': 
			if (this->playerIndex.first == this->mazeState.size() - 1 //out of bounds
				|| this->mazeState[this->playerIndex.first + 1][this->playerIndex.second] == 'Z' //into a barrier
				|| this->mazeState[this->playerIndex.first + 1][this->playerIndex.second] == 'W' //into the wanderer
				|| this->mazeState[this->playerIndex.first + 1][this->playerIndex.second] == 'S') //into the seeker
			{
				std::cout << "This move is invalid. Enter a different move.\n";
				break;
			}
			return this->makePlayerTurnHelper(1, 0); //1 space south would be the first index + 1, the same second index
		case 'E': 
			if (this->playerIndex.second == this->mazeState[0].size() - 1 //out of bounds
				|| this->mazeState[this->playerIndex.first][this->playerIndex.second + 1] == 'Z' //into a barrier
				|| this->mazeState[this->playerIndex.first][this->playerIndex.second + 1] == 'W' //into the wanderer
				|| this->mazeState[this->playerIndex.first][this->playerIndex.second + 1] == 'S') //into the seeker
			{
				std::cout << "This move is invalid. Enter a different move.\n";
				break;
			}
			return this->makePlayerTurnHelper(0, 1); //1 space east would be the same first index, the second index + 1
		case 'W': 
			if (this->playerIndex.second == 0 //out of bounds
				|| this->mazeState[this->playerIndex.first][this->playerIndex.second - 1] == 'Z' //into a barrier
				|| this->mazeState[this->playerIndex.first][this->playerIndex.second - 1] == 'W' //into the wanderer
				|| this->mazeState[this->playerIndex.first][this->playerIndex.second - 1] == 'S') //into the seeker
			{
				std::cout << "This move is invalid. Enter a different move.\n";
				break;
			}
			return this->makePlayerTurnHelper(0, -1); //1 space west would be the same first index, the second index - 1
		case 'O':
			return true; //don't make any modifications to the maze
		case 'Q':
			this->gameOverMessage = "Player chose to quit early.\nSCORE: " + std::to_string(this->playerScore) + "\n";
			return false;
		default:
			std::cout << "Input not recognized. Please enter a valid move";
		}
	}
}

/*helper function for moving N, S, E, or W in makePlayerTurn, which has similar code for all these moves except with different indices. 
  The appropriate index is found using the int value argument passed in through the switch statement in makePlayerTurn().
  The method assumes the move is valid, and checks if it will move onto the goal, a space with treasure, or an empty space. 
  Then, it performs the move and replaces current player position with an empty space*/
bool MazeGame::makePlayerTurnHelper(int leftIndexModifier, int rightIndexModifier)
{
	bool toRet;
	//Based on the character of the position to move to, set the gameOverMessage
	switch (this->mazeState[this->playerIndex.first + leftIndexModifier][this->playerIndex.second + rightIndexModifier])
	{
		case 'G':
			//add points for reaching the goal
			this->playerScore += 30;
			this->gameOverMessage = "You've reached the goal!\nSCORE: " + std::to_string(this->playerScore) + "\n";
			toRet = false; //since this move ends the game, return false
			break;
		case '+':
			this->playerScore += 1;
			toRet = true;
			break;
		case '-':
			this->playerScore -= 1;
			toRet = true;
			break;
		default: 
			//space moved onto is not a valid maze character. End the game immediately
			this->gameOverMessage = "ERROR: This position is not valid. Valid maze characters include '+','-','Z','P','G','W', and 'S'\n";
			return false;
	}
	//now move the player to the new position, set the previous position to an empty space, and update the player's index. Return the appropriate return value
	mazeState[this->playerIndex.first][this->playerIndex.second] = '-';
	mazeState[this->playerIndex.first + leftIndexModifier][this->playerIndex.second + rightIndexModifier] = 'P';
	playerIndex.first += leftIndexModifier;
	playerIndex.second += rightIndexModifier;
	return toRet;
}