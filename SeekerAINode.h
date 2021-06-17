#pragma once
#include<vector>
#include<unordered_map>
#include<cmath>
#include<string>
#include<iostream>

//forward declaration so SeekerAINode can declare seekerCompare struct a friend, so it can access private attributes in the operator overload
struct seekerCompare;


/*Class used for nodes in a tree used to implement the A* algorithm. Used so the seeker AI can determine which direction to move on its turn (N, S, E, or W) in*/
class SeekerAINode
{
	friend seekerCompare;
	friend void deleteSeekerTree(SeekerAINode*);
private:
	SeekerAINode* parent; //pointer to parent node
	SeekerAINode* nChild; //pointer to node representing a move N from this location
	SeekerAINode* sChild; //pointer to node representing a move S from this location
	SeekerAINode* eChild; //pointer to node representing a move E from this location
	SeekerAINode* wChild; //pointer to node representing a move W from this location
	char direction; //Which move from the previous node led to this one (N, S, E, or W). Use 'R' for the root node
	int realDist; //distance travelled to get to this location. Int and not float since distance between spaces is 1
	double heurDist; //estimated (straight-line) distance to destination. Is a float since straight line distance calculation may not be a whole number.
	double totalDist; //realDist + heurDist, estimated total distance of path from starting location to goal
	std::pair<int, int> seekerIndex; //Index of the seeker after reaching this point in a path
	std::pair<int, int> goalIndex; //Index of the goal, which the seeker cannot move to
	std::pair<int, int> wanderIndex; //Index of the wanderer enemy, which the seeker cannot share a space with
	std::pair<int, int> playerIndex; //Index of the player, which is the end goal for this algorithm to reach
	std::vector<std::vector<char>>* maze; //Store pointer to the maze this tree is searching for paths for
public:
	//constructor, taking in a reference to the parent node of this new node, the seker's location, and the goal and wanderer locations. Also takes in a pointer to the maze
	SeekerAINode(SeekerAINode* parent, char direction, std::pair<int, int> seekerIndex, std::pair<int, int> goalIndex, std::pair<int, int> wanderIndex, std::vector<std::vector<char>>* maze);
	std::pair<int, int> getSeekerIndex() { return seekerIndex; } //getter for seekerIndex int pair
	SeekerAINode* getParent() { return parent; } //getter for parent
	char getDirection() { return direction; } //getter for direction
	int getRealDist() { return realDist; } //getter for realdist
	double getTotalDist() { return totalDist; } //getter for totalDist
	//Create all child nodes for this node, and return pointers to them so that they can be placed in the priority queue of nodes. Pass in a map of visited locations so a previously visited location will not be calculated again.
	//Map key is a string containing the int pair for the index of the previously visited location, and the value of the map will be shortest distance found to get to that location
	std::vector<SeekerAINode*> expand(const std::unordered_map<std::string, int> &locationsVisited);
};


//comparison struct to use with priority_queue of SeekerAINode pointers
//Information about comparitors: http://neutrofoton.github.io/blog/2016/12/29/c-plus-plus-priority-queue-with-comparator/ 
struct seekerCompare
{
	bool operator()(SeekerAINode* left, SeekerAINode* right) {	return left->totalDist > right->totalDist;	}
};

//function to delete every node in the tree when finished
void deleteSeekerTree(SeekerAINode* node);