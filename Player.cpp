#include "Player.h"

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

/**
 * Creates one string from Players name and points
 * @param none
 * @return string of playerName_points
*/
std::string Player::Stringify(){
    std::string str = name_ + "_" + std::to_string(points_);
    return str;
}

// std::string Player::ToRelativePosition(Position other,std::vector<std::string> possible_moves){

// }

