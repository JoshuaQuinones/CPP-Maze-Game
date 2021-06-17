#pragma once
#include <queue>
#include "SeekerAINode.h"
/*Class used to store information to determine what move the seeker will make on its next turn.
  create a tree that will be used with the A* algorithm to determine the shortest path to the player, which will be used to determine which move the seeker should make on its turn.*/


class SeekerAI
{
private:
	std::pair<int, int> seekerIndex; //current index of the seeker in the maze
	std::vector<std::vector<char>>* mazeP; //pointer to maze that this seekerAI is associated with
	char standingOver; //keep track of what kind of space the seeker is currently standing over, a + space or a - space
public:
	SeekerAI(std::pair<int, int> index, std::vector<std::vector<char>>* maze) : seekerIndex(index), mazeP(maze), standingOver('-') {} //Constructor. Take in the seeker index. The starting position of the seeker will be a '-' space
	//Use A* algorithm and tree to determine which turn the seeker will make, perform it, and return the seeker's new location. Take in playerIndex, seekerIndex, and goalIndex to pass to SeekerAINodes
	std::pair<int, int> makeTurn(std::pair<int, int> playerIndex, std::pair<int, int> wanderIndex, std::pair<int, int> goalIndex); 
	char getStandingOver() { return standingOver; } //getter for standingOver
};

