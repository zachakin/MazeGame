#include "Maze.h"
#include "Player.h"
#include <iostream>
#include <time.h>           // library for rand() and srand() to randomize maze square values  
#include <vector>
#include <queue>               

/**
 * Converts squaretype of the parameter into a string to output on the board
 * @param SquareType sq
 * @return String of converted squaretype
*/
std::string SquareTypeStringify(SquareType sq){
    if(sq == SquareType::Empty){
		return empty_emoji; 		
	} else if(sq == SquareType::Human){
		return human_emoji;					
	} else if(sq == SquareType::Exit){	
		return exit_emoji; 				
	} else if(sq == SquareType::Wall){
		return wall_emoji;				     
	} else if(sq == SquareType::Treasure){
		return treasure_emoji;				
	} else{
		return enemy_emoji;							
	}
}

/**
 * Default constructor
 * Creates a Board object with 4 rows and 4 columns
 * Row 0 Column 0 is set to the human Player's starting position
 * Row 3 Column 3 is set to the exit position of the maze
 * [0][1] through [3][2] are randomized with a 20% chance of being a wall,
 * 10% chance of being treasure, and otherwise empty
 * If [0][1] is a wall then [1][0] cannot be a wall for the maze to be solvable
 * If [2][3] is a wall then [3][2] cannot be a wall for the maze to be solvable
 * @param none
 * @return none
*/
Board::Board(){
    rows_=4;
    cols_=4;
    Position start;
    Position exitSpot;
    start.row=0;
    start.col=0;
    exitSpot.row=3;
    exitSpot.col=3;
    SetSquareValue(start,SquareType::Human);
    SetSquareValue(exitSpot,SquareType::Exit);

    int x;
    srand(time(NULL));
    for(int i=0;i<4;i++){                           // for each row
        for(int j=0;j<4;j++){                       // for each column
            if(!(i==0&&j==0)&&!(i==3&&j==3)){       // conditional to set squares other than start and exit
                Position pos;                       // Position of current square getting set
                pos.row = i;
                pos.col = j;
                x = rand()%100;                     // Random value 0 to 99
                if((i==1&&j==0)||(i==3&&j==2)){     // conditional to make sure maze is solvable
                    Position diagonal;
                    diagonal.row = j;
                    diagonal.col = i;
                    if(get_square_value(diagonal)==SquareType::Wall){   
                        if(x>=20&&x<30){
                            SetSquareValue(pos,SquareType::Treasure);
                        } else {
                            SetSquareValue(pos,SquareType::Empty);
                        }
                    }
                } else {
                    if(x>=0&&x<20){                                 // 20% chance of being a wall
                        SetSquareValue(pos,SquareType::Wall);
                    } else if(x>=20&&x<30){
                        SetSquareValue(pos,SquareType::Treasure);   // 10% chance of being treasure
                    } else {
                        SetSquareValue(pos,SquareType::Empty);      // else square is empty
                    }
                }
            }
        }
    }
}

/**
 * Returns the squaretype of the given position
 * @param Position pos: position that the function is returning the square value of
 * @return SquareType value
*/
SquareType Board::get_square_value(Position pos) const{
    return arr_[pos.row][pos.col];
}

/**
 * Sets the squaretype of the given position
 * @param Position pos: position on board where its squartype is being set, SquareType value: value position's squaretype is being set to
 * @return none
*/
void Board::SetSquareValue(Position pos, SquareType value){
    arr_[pos.row][pos.col] = value;
}

/**
 * Checks coordinates around the Player's position to check if they are within the edges of the board
 * If the coordinates are on the board and the coordinates are not a wall or an enemy, then
 * the coordinates are a valid move, and the Position and direction (right,left,up,down)
 * are pushed to separate vectors
 * @param Player *p: points to the Player that the function is checking for avaible moves
 * @return vector<Position>, returns a vector of Positions that are valid moves for the Player
*/
std::vector<Position> Board::GetMoves(Player *p){
    Position pos = p->get_position();
    std::vector<Position> moves;
    Position push = pos;
    moves_strings.clear();
    // check players position and
    // see what moves can be made
    // return vector of positions it can move to
    if(pos.row-1>=0){
        push.col = pos.col;
        push.row = pos.row-1;
        if(get_square_value(push)!=SquareType::Wall&&get_square_value(push)!=SquareType::Enemy){
            moves.push_back(push);
            moves_strings.push_back("UP");
        }
    }
    if(pos.row+1<=3){
        push.col = pos.col;
        push.row = pos.row + 1;
        if(get_square_value(push)!=SquareType::Wall&&get_square_value(push)!=SquareType::Enemy){
            if(!p->is_human()&&get_square_value(push)==SquareType::Exit){
            } else {
                moves.push_back(push);
                moves_strings.push_back("DOWN");
            }
            
        }
    }
    if(pos.col-1>=0){
        push.col = pos.col - 1;
        push.row = pos.row;
        if(get_square_value(push)!=SquareType::Wall&&get_square_value(push)!=SquareType::Enemy){
            moves.push_back(push);
            moves_strings.push_back("LEFT");
        }
    }
    if(pos.col+1<=3){
        push.col = pos.col + 1;
        push.row = pos.row;
        if(get_square_value(push)!=SquareType::Wall&&get_square_value(push)!=SquareType::Enemy){
            if(!p->is_human()&&get_square_value(push)==SquareType::Exit){
            } else {
                moves.push_back(push);
                moves_strings.push_back("RIGHT");
            }
        }
    }
    return moves;
}

/**
 * Checks if the Player can be moved successfully
 * Checks if the Player is moving to a Position where there is treasure and adds points accordingly
 * Sets the players previous position SquareType to Empty and its new position SquareType to Human
 * @param Player *p: pointer to the player that is being moved, Position pos: position the player is being moved to
 * @return returns true if the Player was moved successfully, false otherwise
*/
bool Board::MovePlayer(Player *p, Position pos){
    Position temp = p->get_position();
    if(!p->is_human()&&get_square_value(pos)==SquareType::Enemy){
        return false;
    }
    p->SetPosition(pos);
    if(p->get_position()==temp){
        return false;
    } else {
        if(p->is_human()&&get_square_value(pos)==SquareType::Treasure){
            std::cout << "Treasure found! +100 Points" << std::endl;
            p->ChangePoints(100);
        }
        SetSquareValue(pos,get_square_value(temp));
        SetSquareValue(temp,SquareType::Empty);
        return true;
    }
}

/**
 * Returns the squaretype of exit position to check if the player has made it there yet
 * @param none
 * @return SquareType value
*/
SquareType Board::GetExitOccupant(){
    return arr_[3][3];
}

/**
 * Prints the board by checking the SquareType values at each position
 * @param Board b: the board that is being printed
 * @return none
*/
void Maze::PrintBoard(Board b){
    Position pos;
    for(int i=0;i<4;i++){
        std::cout<<std::endl;
        for(int j=0;j<4;j++){
            pos.row = i;
            pos.col = j;
            std::cout << SquareTypeStringify(b.get_square_value(pos));   
            if(b.get_square_value(pos)==SquareType::Wall){
                std::cout<< " ";
            }
        }
    }
    std::cout<<std::endl;
}

/**
 * Default Constructor
 * Creates a new Maze object
 * Sets the value of the board pointer to a new board object and sets the turn_count_ of the human player to 0
 * @param none
 * @return none
*/
Maze::Maze(){
    board_ = new Board();
    turn_count_=0;
}

/**
 * Adds the human player to the players_ vector
 * Creates new enemy players ensuring that their position allows the human player to have an available initial move
 * Enemies are given unique positions on rows 1 and 2 in any column
 * If the enemy position not allowed to be set to a position, its is placed in the bottom row
 * Enemies are given names enemy_n
 * Enemy players is_human_ are set to false
 * Enemy players are pushed to the players_ vector
 * @param Player *human: pointer to the human player in the new game, const int enemies: tells the function how many enemies to create
 * @return none
*/
void Maze::NewGame(Player *human, const int enemies){
    srand(time(NULL));
    players_.push_back(human);
    Position enemyPos;
    Position noMovesWall;
    noMovesWall.row = 0;
    noMovesWall.col = 1;
    for(int i=0;i<enemies;i++){
        Player* e = new Player();
        e->set_name("enemy_"+std::to_string(i+1));
        e->set_is_human(false);
        enemyPos.row = rand()%2 + 1;
        enemyPos.col = rand()%4; 
        if(board_->get_square_value(noMovesWall)==SquareType::Wall&&enemyPos.row==1&&enemyPos.col==0){
            enemyPos.row++;
        }

        bool duplicatePos = false;
        for(int i=0;i<players_.size();i++){
            if(enemyPos==players_[i]->get_position()){
                duplicatePos = true;
            }
        }
        if(duplicatePos){
            enemyPos.row = 3;
            if(enemyPos.col==3) enemyPos.col--;
        }
        board_->SetSquareValue(enemyPos,SquareType::Enemy);
        e->SetPosition(enemyPos);
        players_.push_back(e);
    }
}


/**
 * Prints the board and a list of the possible moves for the player
 * If no moves are available the player loses their turn
 * Takes in user input of direction to move, and keeps prompting the user for input until the direction is valid
 * The player is then moved to the Postion in the direction they chose
 * if the player is human, their turn_count_ is incremented by 1
 * @param Player *p: pointer to the player who is taking their turn
 * @return none
*/
void Maze::TakeTurn(Player *p){
    PrintBoard(*board_);
    std::vector<Position> coordinates = board_->GetMoves(p);
    std::vector<std::string> moves = board_->get_moves_strings();
    bool found = false;
    int found_index;

    if(p->is_human()){
        std::cout << p->get_name() << " can move: ";
        if(moves.size()>0){
            for(int i=0;i<moves.size();i++){                    // Lists possible moves
                std::cout<< moves[i] << " ";    
            }
            std::cout << std::endl;

            while(!found){
                std::string input;
                std::cout<<"Enter direction to move: "; 
                getline(std::cin,input);                        // User input

                for(int i=0;i<input.size();i++){                // Makes user input case insensitive
                    input[i]=std::toupper(input[i]);
                }

                for(int i=0;i<moves.size();i++){                // Checks if user input is a valid direction
                    if(input==moves[i]){
                        found = true;
                        found_index = i;                        // Saves the index of the coordinates the user wants to move to
                    }
                }

                if(!found){
                    std::cout << "Invalid direction, try again." << std::endl;
                }
            }
            Position move_to;
            move_to.row = coordinates[found_index].row;
            move_to.col = coordinates[found_index].col;

            board_->MovePlayer(p,move_to);
            if(p->is_human()) turn_count_++;

        } else {
            std::cout << "NO MOVES AVAILABLE" << std::endl;
        }
    } else {
        // Using BFS to find the shortest path to the human player
        build_graph();
        int human_pos_key = players_[0]->get_position().row*4 + players_[0]->get_position().col + 1;
        int source_pos_key = p->get_position().row*4 + p->get_position().col + 1;
        Position move_to;
        if(mg.findShortestPath(human_pos_key,source_pos_key)==-1){
            move_to = p->get_position();
        } else {    
            move_to = mg.findPath(human_pos_key,source_pos_key);
        }
        std::string enemy_direction;
        if(move_to.row==p->get_position().row-1){
            enemy_direction = "UP";
        } else if(move_to.row==p->get_position().row+1){
            enemy_direction = "DOWN";
        } else if(move_to.col==p->get_position().col-1){
            enemy_direction = "LEFT";
        } else if(move_to.col==p->get_position().col+1){
            enemy_direction = "RIGHT";
        }
        if(board_->MovePlayer(p,move_to)){
            std::cout<<p->get_name()<<" moved " << enemy_direction << std::endl;
        } else {
            std::cout<<p->get_name()<<" could not move " << std::endl;
        }
    }
    return;
}

/**
 * Returns pointer to the Player object of the player whose turn is next
 * Checks if the current player is last in the players_ vector
 * if that is the case, it is the first player is the players_ vector's turn
 * @param Player *p: pointer to the current Player
 * @return Player*
*/
Player* Maze::GetNextPlayer(Player *p){
    // Players vector
    for(int i=0;i<players_.size();i++){
        if(p->get_name()==players_[i]->get_name()){
            if(i==players_.size()-1){
                return players_[0];
            } else {
                return players_[i+1];
            }
        } 
    }
    return NULL;
}

/**
 * Returns true if the human player made it to the exit position
 * Returns false if the human player has not made it to the exit position yet
 * Also returns false if an enemy player moves onto the position of the human player
 * @param none
 * @return bool: true if the game is over, false otherwise
*/
bool Maze::IsGameOver(){
    if(board_->GetExitOccupant()==SquareType::Human){
        won = true;
        return true;
    }
    if(board_->get_square_value(players_[0]->get_position())==SquareType::Enemy){
        won=false;
        return true;
    }
    return false;
}

/**
 * Uses the won boolean variable to check if the player won or lost then
 * returns a sentence based on the result of the game
 * @param none
 * @return string: report of if the player won or lost, how many turns it took, and how many points the player had
*/
std::string Maze::GenerateReport(){
    std::string report;
    if(won){
        turn_count_++;
        PrintBoard(*board_);
        report = "You win! It took you " + std::to_string(turn_count_) + " turns, and you earned " + std::to_string(players_[0]->get_points()) + " points.";
    } else {
        PrintBoard(*board_);
        report = "You lost after " + std::to_string(turn_count_) + " turn(s). Better luck next time!";
    }
    return report;
}

/**
 * This function puts the 4x4 board into a graph data structure
 * It creates vertices and edges between vertices if the squaretype is not a wall or exit
 * The vertices of the graph are numbered left to right by row from 1 to 16
 * @param none
 * @return none
*/
void Maze::build_graph(){
    int key = 1;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            Position pos;
            pos.row = i;
            pos.col = j;
            if(board_->get_square_value(pos)!=SquareType::Wall&&board_->get_square_value(pos)!=SquareType::Exit){
                mg.addVertex(key,pos);
            }
            key++;
        }
    }
    key=1;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            Position pos;
            pos.row=i;
            pos.col=j;
            if(board_->get_square_value(pos)!=SquareType::Wall&&board_->get_square_value(pos)!=SquareType::Exit){
                int source = pos.row*4 + pos.col + 1;
                if(key%4==0){
                    mg.addEdge(source,source+4);
                } else {
                    mg.addEdge(source,source+1);
                    mg.addEdge(source,source+4);
                }
            }
            key++; 
        }
    }
}   


/**
 * function to add edge between two vertices
 * @param int v1,v2: values of the keys of vertices that are getting connected
 * @return none
*/
void Graph::addEdge(int v1, int v2){

    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i]->key == v1){
            for(int j = 0; j < vertices.size(); j++){
                if(vertices[j]->key == v2 && i != j){
                    adjVertex av;
                    av.v = vertices[j];
                    vertices[i]->adj.push_back(av);
                    //another vertex for edge in other direction
                    adjVertex av2;
                    av2.v = vertices[i];
                    vertices[j]->adj.push_back(av2);
                }
            }
        }
    }
}

/**
 * function to add a vertex to the graph
 * @param int n: value of the key of the new vertex, Position pos: position of the new vertex
 * @return none
*/
void Graph::addVertex(int n,Position pos){
    bool found = false;
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i]->key == n){
            found = true;
        }
    }
    if(found == false){
        vertex * v = new vertex;
        v->key=n;
        v->pos_=pos;
        vertices.push_back(v);
    }
}

/**
 * function that finds the steps for the shortest path
 * @param int src: value of key of human player vertex, int dest: value of key of enemy player vertex;
 * @return Position of the next step the enemy should take towards the source
*/
Position Graph::findPath(int src, int dest)
{
    // the "path" vector stores the vertices of the shortest path
    std::vector<vertex*> path;

    // A pointer for destination vertex
    vertex *vDest;

    for(int i=0;i<vertices.size();i++){
        if(vertices[i]->key==dest){
            vDest=vertices[i];
        }
    }
    while(true){
        path.insert(path.begin(),vDest);
        if(vDest->key==src) break;
        vDest=vDest->pred;
    }
    // std::cout<< "The shortet path is: ";

    // for(int i=path.size()-1;i>=0;i--){
    //     std::cout << path[i]->key << " ";
    // }
    // std::cout<<std::endl;

    return path[path.size()-2]->pos_;   
}

/**
 * function that uses breadth first search to find the shortest path from vertex src to vertex dest
 * @param int src: value of key of human player vertex, int dest: value of key of enemy player vertex;
 * @return int, returns -1 if there is no path, otherwise returns the number of steps from src to dest
*/
int Graph::findShortestPath(int src, int dest){

    // Need to find the source vertex since we only have it's key 'src'
    // A pointer for source vertex
    vertex *vStart;

    for(int i = 0; i < vertices.size(); i++)
    {
        vertices[i]->visited=false;
        if(vertices[i]->key == src)
        {
            vStart = vertices[i];
        }
    }

    vStart->visited = true;

    // Use the queue to keep track of visited vertices
    std::queue<vertex*> q;

    // Enqueue the source vertex
    q.push(vStart);

    vertex *n;

    // standard BFS
    while(!q.empty()){
        n = q.front();
        q.pop();
        // go to all the adjacent vertices of n
        for( int x = 0; x < n->adj.size(); x++ )
        {
            // If a adjacent has not been visited, then mark it visited and enqueue it
            // Update the distance of the adjacent vertices along the way
            // Stop when you reach the destination vertex and return the needful
            if(n->adj[x].v->visited==false){
                n->adj[x].v->visited=true;
                n->adj[x].v->distance=n->adj[x].v->distance+1;
                n->adj[x].v->pred=n;
                q.push(n->adj[x].v);
                if(n->adj[x].v->key==dest){
                    return x;
                }
            }
        }
    }
    return -1;
}