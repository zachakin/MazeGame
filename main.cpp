/*
Zach Akin-Amland
CSCI 3010
HW 1 - Maze Game

This is the main file for the Maze Game that
implements the Maze, Board, and Player classes
A player is initialized and given a name and starting position of [0][0]
A maze object is initialized and a new game is created
The game runs until game ending conditions have been met
The results of the game are outputted after the game has ended

I implemented feature number 1 from Step 2 of the write up
I made it so walls and enemies do not spawn in locations that make the maze unsolvable
such as two non-empty ( and non-treasure ) SquareTypes adjacent to the starting position or 
adjacent to the exit position that block off the maze path

For the extra credit, I put the board into a graph data structure,
the enemy strategy is determined by using a breadth first search to find
the shortest path from vertex A ( enemy position) to vertex B (human position) after each turn. If the enemy's 
shortest path to the human is blocked, then it does not move.

Commands to run the program:
$ make clean
$ make all
$ ./maze

*/
#include "Player.h"
#include "Maze.h"
#include <iostream>

int main(){
    Player user("Player1", true);
    Position start;
    start.col=0;
    start.row=0;
    user.SetPosition(start);

    Maze m = Maze();
    m.NewGame(&user,2);
    Player* currentPlayer = &user;
    while(!m.IsGameOver()){
        m.TakeTurn(currentPlayer);
        currentPlayer = m.GetNextPlayer(currentPlayer);
    }
    std::cout<< m.GenerateReport() << std::endl;
    return 0;
}