#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include "SeekerAI.h"

/*MazeGame class is used to create and play a maze game, where the player moves their character towards a goal by taking turns. The player can collect treasure to increase their score and must avoid enemies. 
  When the player reaches the goal, is caught by an enemy, or choses to quit on their turn, the game will end. */
class MazeGame
{
private:
	std::vector<std::vector<char>> mazeState; //2D vector that will store the characters that make up the maze
	std::pair<int, int> playerIndex; //Index of the player's position on the board
	std::pair<int, int> goalIndex; //Index of the goal on the board
	std::pair<int, int> wanderIndex; //Index of the wanderer enemy on the board
	std::pair<int, int> seekerIndex; //Index of the seeker enemy on the board
	std::string gameOverMessage; //Message that tells how the game ended (if the player quit, was caught by an enemy, or reached the goal)
	int playerScore; //Stores the player's current score. Score changes depending on what kind of space the player moves onto
	char wanderStandingOver; //Stores what space the wanderer is on top of (- or +)
	//functions:
	void displayBoard(); //Method to display the current state of the maze. Private, will be used by playGame method while playing
	bool makeWanderTurn(); //Determine the wanderer's next move, which is random, and perform it. Return true if the Wanderer catches the player
	bool makePlayerTurn(); //Method to move the player's position according to their input (North, South, East, West). Will return true if the player performs a move that will continue the game and returns false if the game ends.
	bool makePlayerTurnHelper(int, int); //Method to use with makePlayerTurn() to perform a move for the player to move north, south, east, or west.
public:
	MazeGame(); //Constructor. Takes user input to get the maze file's name, read the maze from the file into the 2D vector, find the player's starting position, and set the initial score
	void playGame(); //Begins the game and will take player moves until the game ends naturally or the player quits
	void resetMaze(); //Takes user input to read a file and create the maze. Sets all values to match this maze
};