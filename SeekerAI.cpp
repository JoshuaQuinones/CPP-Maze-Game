#include "SeekerAI.h"
/*Implement the methods of the SeekerAI class*/

/*Use the A* algorithm to determine what the shortest path to the player is, and then make the first move of that path. Return the seeker's new position*/
std::pair<int, int> SeekerAI::makeTurn(std::pair<int, int> playerIndex, std::pair<int, int> wanderIndex, std::pair<int, int> goalIndex)
{
	std::unordered_map<std::string, int> locationsVisited; //keep track of locations visited while performing the algorithm
	std::priority_queue<SeekerAINode*, std::vector<SeekerAINode*> ,seekerCompare> openNodes; //keep track of open nodes with a priority queue, removing the node with the smallest totalDist each time
	//create root node of tree and place in priority queue
	SeekerAINode* rootNode = new SeekerAINode(nullptr, 'R', seekerIndex, goalIndex, wanderIndex, this->mazeP);
	//use while loop to perform the algorithm. Stop searching for a path when the goal is found or there are no more available open nodes
	SeekerAINode* currentNode; //what node is currently being expanded/looked at
	bool playerFound = false;
	//expand the root node and place it's children in the priority queue, add its index to visited locations
	std::vector<SeekerAINode*> newNodes;
	std::string forMap(std::to_string(rootNode->getSeekerIndex().first) + "," + std::to_string(rootNode->getSeekerIndex().second));
	locationsVisited.insert({ forMap, rootNode->getRealDist() });
	newNodes = rootNode->expand(locationsVisited);
	for (SeekerAINode* n : newNodes)
	{
		openNodes.push(n);
	}
	//change current node to smallest item in priority queue
	currentNode = openNodes.top();
	while (!playerFound && !openNodes.empty())
	{
		openNodes.pop(); //pop previous shortest path node here at start of loop, otherwise wouldn't be able to enter loop if only one element was in openNodes
		//first, check that the current location being considered is not the player's location
		if (currentNode->getSeekerIndex() == playerIndex)
		{
			playerFound = true;
			break; //exit the while loop
		}
		//place current node's seekerIndex in the unordered_map as a string (for the key value)
		std::string forMap(std::to_string(currentNode->getSeekerIndex().first) + "," + std::to_string(currentNode->getSeekerIndex().second));
		locationsVisited.insert({ forMap, currentNode->getRealDist() }); //REMINDER: needs to be in curly brackets. https://www.geeksforgeeks.org/unordered_map-insert-in-c-stl/
		//expand the current node and place all of its children in the priority queue
		std::vector<SeekerAINode*> newNodes;
		newNodes = currentNode->expand(locationsVisited);
		for (SeekerAINode* n : newNodes)
		{
			openNodes.push(n);
		}
		//change the current node to be the next node (with the smallest estimated totalDist) in the priority queue
		if (!openNodes.empty())
		{
			currentNode = openNodes.top();
		}
	}
	//see if loop was exited because the player was found. If so, go up the path to the node that's the child of the root node, and return the move that was made to it (the first move in the path)
	if (playerFound)
	{
		while (currentNode->getParent()->getDirection() != 'R')
		{
			currentNode = currentNode->getParent();
		}
		//perform the seeker's turn in the maze and update it's position
		char toRet = currentNode->getDirection(); //copy the char since we will delete the nodes
		deleteSeekerTree(rootNode); //delete the tree to avoid memory leak
		(*this->mazeP)[this->seekerIndex.first][this->seekerIndex.second] = standingOver; //set seeker's current position to what it was before moving here
		switch (toRet) //update position
		{
			case 'N':
				std::cout << "SEEKER'S TURN: The seeker moves north\n";
				this->seekerIndex.first -= 1;
				break;
			case 'S':
				std::cout << "SEEKER'S TURN: The seeker moves south\n";
				this->seekerIndex.first += 1;
				break;
			case 'E':
				std::cout << "SEEKER'S TURN: The seeker moves east\n";
				this->seekerIndex.second += 1;
				break;
			case 'W':
				std::cout << "SEEKER'S TURN: The seeker moves west\n";
				this->seekerIndex.second -= 1;
				break;
			default:
				throw "Result of SeekerAI.makeTurn is not a valid char";
		}
		//see if the seeker's new position is over the player's position
		if ((*this->mazeP)[this->seekerIndex.first][this->seekerIndex.second] == 'P')
		{
			//return <-1, -1>, which is an otherwise impossible value, to show that the player has been caught
			return std::pair<int, int>(-1, -1);
		}
		//replace standingOver based on seeker's current position, then replace the char with 'S'
		standingOver = (*this->mazeP)[this->seekerIndex.first][this->seekerIndex.second];
		(*this->mazeP)[this->seekerIndex.first][this->seekerIndex.second] = 'S';
		return this->seekerIndex;
	}
	else //seeker could not find the player. Will skip it's turn
	{
		std::cout << "SEEKER'S TURN: Seeker could not find the player and will do nothing. Skipping its turn\n";
		return this->seekerIndex;
	}
}

