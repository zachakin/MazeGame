#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include "Player.h"

// you may change this enum as you need
enum class SquareType { Wall, Exit, Empty, Human, Enemy, Treasure };

// TODO: implement
// this function should return a string representation of a given SquareType
// for example an ascii letter or an emoji
std::string SquareTypeStringify(SquareType sq);

const std::string empty_emoji="\U00002B1C";               // white square
const std::string human_emoji="\U0001F680";               // rocket ship
const std::string exit_emoji="\U0001F30E";          // Earth
const std::string wall_emoji = "\U0001FAA8";        // Rock
const std::string treasure_emoji = "\U00002B50";    // Star
const std::string enemy_emoji = "\U0001F47D";       // Alien

class Board {
	public:
		// TODO: implement
		Board();
		
		// already implemented in line
		int get_rows() const {return 4; }  // you should be able to change the size of your 
		int get_cols() const {return 4; }  // board by changing these numbers and the numbers in the arr_ field

		// TODO: you MUST implement the following functions
		SquareType get_square_value(Position pos) const; 

		// set the value of a square to the given SquareType
		void SetSquareValue(Position pos, SquareType value);

		// get the possible Positions that a Player could move to
		// (not off the board or into a wall)
		std::vector<Position> GetMoves(Player *p);

		// Move a player to a new position on the board. Return
		// true if they moved successfully, false otherwise.
		bool MovePlayer(Player *p, Position pos);

		// Get the square type of the exit square
		SquareType GetExitOccupant();

		// You probably want to implement this
		// friend std::ostream& operator<<(std::ostream& os, const Board &b);

		// Get a vector of strings that represent possible moves ( "RIGHT" , "LEFT" , "UP" , "DOWN")
		std::vector<std::string> get_moves_strings() { return moves_strings;}


	private:
		SquareType arr_[4][4];
		std::vector<std::string> moves_strings;
		
		int rows_; // might be convenient but not necessary
		int cols_;
		
		// you may add more fields, as needed
};  // class Board

struct vertex;

struct adjVertex{
    vertex *v;
};

struct vertex{
	int key;
	Position pos_;
    bool visited = false;
    int distance = 0;
    vertex *pred = NULL; // predecessor
    std::vector<adjVertex> adj;
};

class Graph
{
    public:
        void addEdge(int v1, int v2);			// adds edge between 2 vertices
        void addVertex(int n, Position pos);	// adds vertex to the graph
        int findShortestPath(int sourceVertex, int destinationVertex);	// finds shorted number of steps to get from source to destination
        Position findPath(int sourceVertex, int destinationVertex);		// find individual steps to get from source to destination
        std::vector<vertex*> vertices;			// vector of vertices in the graph

};	// class Graph

class Maze {
public:
	// TODO: implement these functions
	Maze(); // constructor

	// initialize a new game, given one human player and 
	// a number of enemies to generate
	void NewGame(Player *human, const int enemies);

	// have the given Player take their turn
	void TakeTurn(Player *p);

	// Get the next player in turn order
	Player * GetNextPlayer(Player* p);

	// return true iff the human made it to the exit 
	// or the enemies ate all the humans
	bool IsGameOver();

	// You probably want to implement these functions as well
	// string info about the game's conditions after it is over
	std::string GenerateReport();
	// friend std::ostream& operator<<(std::ostream& os, const Maze &m);

	void PrintBoard(Board b);
	void build_graph();
private:
	Board *board_; // HINT: when you instantiate your board_, use the new Board() syntax
	std::vector<Player *> players_;
	int turn_count_;
	bool won = false;	// condition that is true if and only if the human player has made it to the exit position
	Graph mg;

	// you may add more fields, as needed

};  // class Maze

#endif  // MAZE_H



