#include "Player.h"
#include <vector>

/**
 * Default constructor
 * Creates a Player object with an empty name, false is_human_, and 0 points
 * @param none
 * @return none
*/
Player::Player(){
    name_="";
    is_human_=false;
    points_=0;
}

/**
 * Parameterized constructor
 * Creates a Player object a name, boolean value for is_human_, and 0 points
 * @param string name: name of the Player, const bool is_human: boolean value that
 * declares the Player a human or enemy
 * @return none
*/
Player::Player(const std::string name, const bool is_human){
    name_ = name;
    is_human_ = is_human;
    points_ = 0;
}

/**
 * Adds a value to the points of a Player
 * @param const int x: value that is being added to a Player's points
 * @return none
*/
void Player::ChangePoints(const int x){
    points_+=x;
}

/**
 * Sets the position private variable of a Player
 * @param Position pos: position that the Player's position variable is being set to
 * @return none
*/
void Player::SetPosition(Position pos){
    pos_ = pos;
}

bool valid_move(std::string move,std::vector<std::string> possible_moves){
    for(int i=0;i<possible_moves.size();i++){
        if(move==possible_moves[i]) return true;
    }
    return false;
}

/**
 * Creates one string from Players name and points
 * @param none
 * @return string of playerName_points
*/
std::string Player::Stringify(){
    std::string str = name_ + "_" + std::to_string(points_);
    return str;
}

/**
 * Algorithm decides which direction to move the Player towards the other player
 * @param Position other: Position of other player that the function tries to move towards, vector of possible moves
 * @return string of move
*/
std::string Player::ToRelativePosition(Position other,std::vector<std::string> possible_moves){
    std::string move;
    if( other.row > other.col ){
        if(other.row < pos_.row){   // move up
            move = "UP";
        } else {                    // move down
            move = "DOWN";
        }
    } else if( other.row <= other.col ) {
        if(other.col<pos_.col){     // move left
            move = "LEFT";
        } else {                    // move right
            move = "RIGHT";
        }
    }
    while(!valid_move(move,possible_moves)){
        move = possible_moves[rand()%possible_moves.size()];
    }
    return move;

}

