#include "SeekerAINode.h"
/*Implement the methods of the SeekerAINode class*/

/*Constructor. Takes in a pointer to the parent node, the direction taken for this step in the path, and the locations of the seeker, goal, and wanderer*/
SeekerAINode::SeekerAINode(SeekerAINode* parent, char direction, std::pair<int, int> seekerIndex, std::pair<int, int> goalIndex, std::pair<int, int> wanderIndex, std::vector<std::vector<char>>* maze)
{
	this->parent = parent;
	this->seekerIndex = seekerIndex;
	this->goalIndex = goalIndex;
	this->wanderIndex = wanderIndex;
	this->direction = direction;
	this->maze = maze;
	this->nChild = nullptr;
	this->sChild = nullptr;
	this->eChild = nullptr;
	this->wChild = nullptr;
	//if this node's direction is 'R', this is the root node. Its parent should be nullptr
	if (direction == 'R')
	{
		this->realDist = 0; //no distance traveled yet on this path
		this->heurDist = std::sqrt(std::pow(this->playerIndex.first - this->goalIndex.first, 2) + std::pow(this->playerIndex.second - this->goalIndex.second, 2)); //euclidean distance between playerIndex and goalIndex
		this->totalDist = heurDist; //totalDist = heurDist since realDist = 0
	}
	else //this is not the root node
	{
		this->realDist = parent->realDist + 1; //distance between spaces in maze is always 1
		this->heurDist = std::sqrt(std::pow(this->playerIndex.first - this->goalIndex.first, 2) + std::pow(this->playerIndex.second - this->goalIndex.second, 2)); //euclidean distance between playerIndex and goalIndex
		this->totalDist = this->realDist + this->heurDist;
	}
}

/*Create all possible child nodes and return a vector containing pointers to them. Pass in reference to the map of previously visited locations */
std::vector<SeekerAINode*> SeekerAINode::expand(const std::unordered_map<std::string, int>& locationsVisited)
{
	std::vector<SeekerAINode*> toRet;
	//check if a move is possible for all directions, and create a new node for that location if possible. Check if that location already exists in the map of locations provided, first, to avoid repeating calculations for the same locations
	//check North:
	if (this->seekerIndex.first != 0 //won't move out of bounds
		&& (*maze)[this->seekerIndex.first - 1][this->seekerIndex.second] != 'Z' //won't move into a barrier
		&& (*maze)[this->seekerIndex.first - 1][this->seekerIndex.second] != 'W' //won't move into the wanderer
		&& (*maze)[this->seekerIndex.first - 1][this->seekerIndex.second] != 'G') //won't move into the goal
	{
		//check if this location has been visited before
		std::string mapPair(std::to_string(this->seekerIndex.first - 1) + "," + std::to_string(this->seekerIndex.second)); //create string from pair of ints that represent the index after moving north
		if (locationsVisited.find(mapPair) == locationsVisited.end())
		{
			//create the new node and append to vector of nodes to return
			std::pair<int, int> newIndex(this->seekerIndex.first - 1, this->seekerIndex.second);
			this->nChild = new SeekerAINode(this, 'N', newIndex, this->goalIndex, this->wanderIndex, this->maze);
			toRet.push_back(this->nChild);
		}
	}
	//check South
	if (this->seekerIndex.first != maze->size() - 1 //won't move out of bounds
		&& (*maze)[this->seekerIndex.first + 1][this->seekerIndex.second] != 'Z' //won't move into a barrier
		&& (*maze)[this->seekerIndex.first + 1][this->seekerIndex.second] != 'W' //won't move into the wanderer
		&& (*maze)[this->seekerIndex.first + 1][this->seekerIndex.second] != 'G') //won't move into the goal
	{
		//check if this location has been visited before
		std::string mapPair(std::to_string(this->seekerIndex.first + 1) + "," + std::to_string(this->seekerIndex.second)); //create string from pair of ints that represent the index after moving south
		if (locationsVisited.find(mapPair) == locationsVisited.end())
		{
			//create the new node and append to vector of nodes to return
			std::pair<int, int> newIndex(this->seekerIndex.first + 1, this->seekerIndex.second);
			this->sChild = new SeekerAINode(this, 'S', newIndex, this->goalIndex, this->wanderIndex, this->maze);
			toRet.push_back(this->sChild);
		}
	}
	//check East
	if (this->seekerIndex.second != maze->at(0).size() - 1 //won't move out of bounds
		&& (*maze)[this->seekerIndex.first][this->seekerIndex.second + 1] != 'Z' //won't move into a barrier
		&& (*maze)[this->seekerIndex.first][this->seekerIndex.second + 1] != 'W' //won't move into the wanderer
		&& (*maze)[this->seekerIndex.first][this->seekerIndex.second + 1] != 'G') //won't move into the goal
	{
		//check if this location has been visited before
		std::string mapPair(std::to_string(this->seekerIndex.first) + "," + std::to_string(this->seekerIndex.second + 1)); //create string from pair of ints that represent the index after moving east
		if (locationsVisited.find(mapPair) == locationsVisited.end())
		{
			//create the new node and append to vector of nodes to return
			std::pair<int, int> newIndex(this->seekerIndex.first, this->seekerIndex.second + 1);
			this->eChild = new SeekerAINode(this, 'E', newIndex, this->goalIndex, this->wanderIndex, this->maze);
			toRet.push_back(this->eChild);
		}
	}
	//check West
	if (this->seekerIndex.second != 0 //won't move out of bounds
		&& (*maze)[this->seekerIndex.first][this->seekerIndex.second - 1] != 'Z' //won't move into a barrier
		&& (*maze)[this->seekerIndex.first][this->seekerIndex.second - 1] != 'W' //won't move into the wanderer
		&& (*maze)[this->seekerIndex.first][this->seekerIndex.second - 1] != 'G') //won't move into the goal
	{
		//check if this location has been visited before
		std::string mapPair(std::to_string(this->seekerIndex.first) + "," + std::to_string(this->seekerIndex.second - 1)); //create string from pair of ints that represent the index after moving west
		if (locationsVisited.find(mapPair) == locationsVisited.end())
		{
			//create the new node and append to vector of nodes to return
			std::pair<int, int> newIndex(this->seekerIndex.first, this->seekerIndex.second - 1);
			this->wChild = new SeekerAINode(this, 'W', newIndex, this->goalIndex, this->wanderIndex, this->maze);
			toRet.push_back(this->wChild);
		}
	}
	return toRet;
}

/*Recursive function to delete the tree, starting by deleting the children then deleting the current node. If root is passed in, should delete the whole tree*/
//Reference: https://www.geeksforgeeks.org/write-a-c-program-to-delete-a-tree/
void deleteSeekerTree(SeekerAINode* node)
{
	if (node->nChild != nullptr)
		deleteSeekerTree(node->nChild);
	if (node->sChild != nullptr)
		deleteSeekerTree(node->sChild);
	if (node->eChild != nullptr)
		deleteSeekerTree(node->eChild);
	if (node->wChild != nullptr)
		deleteSeekerTree(node->wChild);
	delete node;
}