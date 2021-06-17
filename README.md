# CPP-Maze-Game
A simple maze game written in C++

The player navigates through a grid to reach the goal character 'G'. Along the way, the player 'P' can move over empty spaces '-' that decrease score by one or treausre spaces '+'
that increase score by one. The player cannot move over barrier spaces 'Z' or past the boundaries of the maze. There are two enemies that the player must avoid, the wanderer 'W' 
who moves randomly and the seeker 'S' who will chase the player. The player and the enemies take turns making their moves, and the player enters their moves into the console.
The seeker decides which direction to move in using the A* algorithm for pathfinding, finding the shortest path from its position to the player.

In this repository are all header and source files used to compile the program, a precompiled exe that runs in the console, and 3 maze files that were used to test the program. 
When running, enter the name of the maze file if it is in the same directory as the exe, or enter the path to the maze file ex) "mazes/maze1.txt"
