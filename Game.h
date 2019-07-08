//-----------------------------------------------------------------------------
// Game.h
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
// This is main clas of projects. It represents logic of game Sworms.
//
//-----------------------------------------------------------------------------
//
#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
#include <vector>
#include "Field.h"
#include "Worm.h"
#include "Random.h"

namespace Sep
{
  class Game
  {
  private:
    /// Variables are named according Task description on Wiki page;
    int board_width_; //  width of loaded map;
    int board_height_;
  public:
    /// Program output messages:
    /// Errors:
    static const std::string ERROR_INVALID_TARGET;
    static const std::string ERROR_INVALID_CONFIG_FILE;
    static const std::string ERROR_NO_AMMUNITION;
    static const std::string ERROR_INVALID_PARAMETER;
    static const std::string ERROR_WRONG_PARAMETAR_COUNT;
    static const std::string ERROR_UNKNOWN_COMMAND;
    static const std::string ERROR_COMMAND_NOT_ALLOWED;

    /// Message Info about winner:
    static const std::string END_PLAYER_ONE_WON;
    static const std::string END_PLAYER_TWO_WON;
    static const std::string END_DRAW;

    /// Message Info about shot, chest weapon, move etc.:
    static const std::string WORM_PICKED_UP_BAZOOKA_MESSAGE;
    static const std::string WORM_PICKED_UP_TELEPORT_MESSAGE;
    static const std::string WORM_PICKED_UP_BLOWTORCH_MESSAGE;
    static const std::string WORM_PICKED_UP_MELEE_MESSAGE;
    static const std::string WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
    static const std::string DROWNED_MESSAGE;
    static const std::string FELL_INTO_HIS_DEATH;
    static const std::string AMMUNITION_STATE;

    static const std::string MESSAGE_SHOT_HIT_CHEST;
    static const std::string MESSAGE_SHOT_HIT_EARTH;
    static const std::string MESSAGE_SHOT_HIT_WORM;
    static const std::string MESSAGE_WORM_DIED;
    static const std::string MESSAGE_SHOT_MISSED;
    static const std::string MESSAGE_CANT_MOVE;

    /// Constructor for Game class;
    Game();

    void setBoardWidth(int x_coordinate);

    //-------------------------------------------------------------------------
    //Setter for board_height_ variable
    //param x_coordinate used to set board_height_
    void setBoardHeight(int y_coordinate);

    //-------------------------------------------------------------------------
    // After map is being saved into vector of string, it needs to get form where
    // for each letter in map(A, E, W) it gets it proper field type(AIR, EARTH, WATER).
    // This is done using setter function from Field class and "field" object
    // (field.setType) where field is object and setType is setter function.
    //
    // @param map 2D vector of strings in which is loaded map.
    //
    void setMap(std::vector<std::string> map);

    //-------------------------------------------------------------------------
    // After move is made, this functions setts worm_in_row_ variable to
    // number which represents row(y coordinate) of that new position.
    // @param row new row in which is worm;
    //
    void setWormAtRow(int row);

    //-------------------------------------------------------------------------
    // After move is made, this functions setts worm_in_col_ variable to
    // number which represents column(x coordinate) of that new position.
    // @param col new column in which is worm;
    //
    void setWorAtCol(int col);

    //-------------------------------------------------------------------------
    // Function is used to find size of board.
    // This code was coppied, modified and used, source:
    //
    // https://www.geeksforgeeks.org/split-a-sentence-into-words-in-cpp/
    //
    // @param str sentence which should be checked.
    //
    int removeDupWord(std::string str);



    //-------------------------------------------------------------------------
    // This function prints current status of game(e.g. map).
    // Here is an example:
    //
    // Current Map:\n
    //    0123456789012345678901234567890123456789\n
    //  0                                         0\n
    //  1                                         1\n
    //  2                                         2\n
    //  3           E     EE                      3\n
    //  4          EEE   EEEE                     4\n
    //  5         EEEEEEEEEEEE                    5\n
    //  6    E   EEEEEEEEEEEEEE                   6\n
    //  7   EE  EEEEEEEEEEEEEEEE                  7\n
    //  8  EEE EEEEEEEEEEEEEEEEEE                 8\n
    //  9 EEEEEEEEEEEEEEEEEEEEEEEE                9\n
    // 10WEEEEEEEEEEEEEEEEEEEEEEEEEWWWWWWWWWWWWWW10\n
    //    0123456789012345678901234567890123456789\n
    //
    // Function used only for the first print of the map
    //
    void printMap();

    //------------------------------------------------------------------------------
    /// Getter function for board_width_;
    /// @return value saved in board_width_;
    //
    int getBoardWidth()
    {
      return board_width_;
    };

    //------------------------------------------------------------------------------
    /// Getter function for board_height_;
    /// @return value saved in board_height_;
    //
    int getBoardHeight()
    {
      return board_height_;
    };
    //-------------------------------------------------------------------------
    // Getter function for worm row coordinate;
    // @return value saved in worm_in_row_;
    //
    int getWormAtRow()
    {
      return worm_in_row_;
    };
    //-------------------------------------------------------------------------
    // Getter function for worm row coordinate;
    // @return value saved in worm_in_col_;
    //
    int getWormAtCol()
    {
      return worm_in_col_;
    }

    //-------------------------------------------------------------------------
    //Getter functions for
    int wormIsAtRow(int row);
    int wormIsAtCol(int col);

    //-------------------------------------------------------------------------
    // Function stores file into probe string with push.back function for vectors.
    // File has to start with magic word: "#SWORMS".
    // Whenever there is abbreviation from rules, function will print [ERROR]
    // and return -1 value.
    //
    // If start is good than all possible white spaces before first sybol
    // will be deleted. This is done because it is much easier to
    // parse and check file in this structure.
    //
    // All lines which are irrelevant to file must start with "#".
    // This means that line, which starts with "#" is considered as comment
    // and as that isn't important for process.
    //
    // Each valid file consist of SIZE x y line and MAP: line.
    // If SIZE/MAP isn't present -> return - 1,
    // otherwise proceed.
    //
    // After size is loaded, it is stored in private part of game.h class
    // (board_width_ and board_height_).
    //
    // Each row after word "MAP:" must have same numbers of letters in row
    // as number which is first written after word SIZE.
    // Same is for columns and second number after word SIZE.
    //
    // After all checks were good, function setMap is called.
    // This function will store/copy elements from 2D string vector
    // into 2D FieldType vector.
    // Each symbol will gets it's own FieldType war setted.
    //
    //
    // @param cfg_file_ given configuration file which defines beginn of game.
    // @return -1 to main.cpp if ERROR happened in processing file.
    //           Otherwise return 0(ALL OK).
    //
    int loadConfig(std::string cfg_file_);


    //-------------------------------------------------------------------------
    // This function adds worm on given place(row - coordinate x of map, col - coordinate y of map)
    // First function will check if given field is inside of map, if not than it returns -2;
    //
    // If worm should be placed in water, than nothing happens. Worm sinks inside and
    // nothing changes.
    //
    // If worm is inserted somwhere in air than worm will fall down until some kind of ground
    // is reached(EARTH or WATER or WORM).
    // In that moment fall stops and worm is places right above first symbol unless it is water.
    //
    // After worm is placed 0 is returned.
    // @param row specifies row (y coordinate) on map where worm should be placed.
    // @param col specifies column (x coordinate) on map where worm should be placed.
    // @return -1 if worm couldn't have been placed.
    //           0 if worm is placed correctly
    int addWorm(Worm &worm_to_be_added);



    //--------------------------------------------------------------------------
    // This function moves worms in wanted direction
    // @param was_move specifies wheter the move input was already called for a
    // current worm
    // @param Worm &object object of worm type that is current worm and the
    // one to be moved
    // @param direction second part of input string that tells worm to go
    // right or left
    // @param steps is also taken from user input, used to tell how many steps
    // should worm take
    void move(bool &was_move, Worm &moving_worm,
              std::string direction, int steps);


    //-------------------------------------------------------------------------
    // This function move worm step by step depending on number of steps and
    // direction
    // @param direction says in which direction the current worm should be placed
    // @param object is current worm
    int moveOneStep(int direction, Worm &moving_worm);


    //-------------------------------------------------------------------------
    // This fuction searches for chest above worm after the worm has taken one step
    // @param worm is current worm on play
    void checkForChest(Worm &worm_around_chest);

    //-------------------------------------------------------------------------
    // This function make new map that is to be used. This is a map made of
    // field objects.
    //
    void makeMapOfFields();

    //-------------------------------------------------------------------------
    // This function print the new map of field objects
    void printFinalMap();

    //-------------------------------------------------------------------------
    // This function adds chest in map after every turn with a weapon that worm
    // collects after moving past it or landing on it
    void addChest();

    //-------------------------------------------------------------------------
    // This set worms up by randomizing their places and their names.
    // @param object_1 first worm to be set.
    // @param object_2 second worm to be set.
    // @param object_3 third worm to be set.
    // @param object_4 fourth worm to be set.
    // @param object_5 fifth worm to be set.
    // @param object_6 sixth worm to be set.
    void setupWorms(Worm &worm_one, Worm &worm_two, Worm &worm_three,
                    Worm &worm_four, Worm &worm_five,
                    Worm &worm_six);

    //-------------------------------------------------------------------------
    // This function tries to place worms on their randomized positions.
    // @param player1 vector in which will be stored alive worms of player 1.
    // @param player2 vector in which will be stored alive worms of player 2.
    // @param counter_1 checks how many worms are in vector player1.
    // @param counter_2 checks how many worms are in vector player2.
    void placeWorms(Worm &worm_one, Worm &worm_two, Worm &worm_three,
                    Worm &worm_four, Worm &worm_five, Worm &worm_six,
                    std::vector<Sep::Worm> &player1_worms,
                    std::vector<Sep::Worm> &player2_worms,
                    int &player_one_counter, int &player_two_counter);

    //-------------------------------------------------------------------------
    // This function check if eventualy the game is over with a winner or without
    // winner
    // @param team_1 used to check how many worms have player 1.
    // @param team_2 used to check how many worms have player 2.
    // @param worm_in_game is a current worm, and function checks if he has died on
    // his move.
    // @param number_of_worms_1 tell us how many worms have player 1.
    // @param number_of_worms_2 tell us how many worms have player 2.
    int checkForCasualities(std::vector<Sep::Worm> &team_1,
                            std::vector<Sep::Worm> &team_2,
                            Worm &worm_in_game,
                            int &number_of_worms_1,
                            int &number_of_worms_2);


    //-------------------------------------------------------------------------
    // This function takes input and decides which action should be taken and call
    // other function for executing users input
    // @param action holds user input
    // @param object is worm on play
    // @param vector_1 is vector of worms of player 1
    // @param vector_2 is vector of worms of player 2
    // @param was:move is used to tell us if the move command was already called in
    // the same round for the same worm
    int actionDecider(std::string action, Worm &worm_in_game,
                       std::vector<Sep::Worm> &team_one_worms,
                      std::vector<Sep::Worm> &team_two_worms,
                      bool &was_move);

    //-------------------------------------------------------------------------
    // This function prints the current state when state command is given by user
    // @param object is the current worm
    // @param vector_1 is the vector of worms of player 1
    // @param vector_2 is the vector of worms of player 2
    void state(Worm worm, std::vector<Sep::Worm> worms_team_one,
               std::vector<Sep::Worm> worms_team_two);

    //-------------------------------------------------------------------------
    // This function is called if a user has chosen new current weapon
    // @param weapon is the weapon that has been chosen by on of the players
    // @param object is the current vector
    void choose(std::string weapon, Worm &worm);

    //-------------------------------------------------------------------------
    // This function was copied from internet
    // This function is used to determin wether a user input was a number
    // @param s is part of the user input that needs to be checked for number
    bool is_number(const std::string& s);

    //-------------------------------------------------------------------------
    // This function is used to move worm if a chest or air are directly left or
    // right from the worm that we want to move
    // @param object is the current worm
    // @direction is the directio in which worm should be moved
    int moveWorm(Worm &worm, int direction);

    //-------------------------------------------------------------------------
    // This function add pointers to worms that are succesfuly placed
    // Each field has variable pointer_on_worms, but only field that has the
    // same
    // coordinates as worms points to worm, default value for pointer_on_worm
    // is NULL
    // Function iterates through given vector and add pointer to them
    // @param vector is vector that holds living worms
    void addPointersOnWorms(std::vector<Sep::Worm> &worms_of_player);

    //-------------------------------------------------------------------------
    // This function is called if player want to execute airstrike
    // @param col is the column on which airstrike should be executed
    void airstrike(int col);

    //-------------------------------------------------------------------------
    // This function is called if a player wants to teleport current worm
    // @param row is row coordinate on which a worm should be placed
    // @param col is a column coordinate on which a worm should be placed
    // @param object is a current worm
    int teleport(int row, int col, Worm &worm_to_be_transported);

    Random random;

    enum ReturnValues
    {
      RETURN_DIED_AT_BEGINN = 8,
      RETURN_CONTINUE = 1,
      RETURN_OK = 0,
      RETURN_ERROR_INVALID_CONFIG = -1,
      RETURN_ERROR_INVALID_TARGET = -2,
      RETURN_ERROR_ARGUMENT  = -3,
      RETURN_ERROR_UNKNOWN_COMMAND = -4,
      RETURN_ERROR_WRONG_PARAMETAR_COUNT = -5,
      RETURN_ERROR_INVALID_PARAMETAR = -6,
      RETURN_ERROR_COMMAND_CURRENTLY_NOT_ALLOWED = -7,
      RETURN_ERROR_NO_AMMUNITION = -8,
      RETURN_WARNING_CAN_NOT_MOVE_FURTHER = -9,
      RETURN_INFO_SHOT_MISSED = -10,
      RETURN_INFO_WORM_DROWNED = -11,
      RETURN_INFO_WORM_FELL_INTO_HIS_DEATH = -12,
      RETURN_INFO_WORM_OUT_OF_MAP = -13,
      RETURN_INFO_WORM_DEAD = -14,
      RETURN_INFO_END_WINNER = -15,
      RETURN_INFO_END_DRAW = -16,
      RETURN_QUIT = -17
    };

    //-------------------------------------------------------------------------
    // This function is called if player wants to shoot with bazooka in left
    // upper direction
    // Function shoots and check if some of other wors or other field have been
    // damaged
    int shootBazookaTopLeftCorner();

    //-------------------------------------------------------------------------
    // This function is called if player wants to shoot with bazooka in right
    // upper direction
    // Function shoots and check if some of other wors or other field have been
    // damaged
    int shootBazookaTopRightCorner();

    //-------------------------------------------------------------------------
    // This function is called if player wants to shoot with bazooka in left under
    // direction
    // Function shoots and check if some of other wors or other field have been
    // damaged
    int shootBazookaDownLeftCorner();

    //------------------------------------------------------------------------
    // This function is called if player wants to shoot with bazooka in right under
    // direction
    // Function shoots and check if some of other wors or other field have been
    // damaged
    int shootBazookaDownRightCorner();

    //-------------------------------------------------------------------------
    // This function is called if player wants to shoot with bazooka in the middle of
    // the map. That means the shot is not placed anywhere near the edges
    // Function shoots and check if some of other wors or other field have been
    // damaged
    // @param row is the row coordinate of the first field that has been hit
    // @param col is the column coordinate of the first field that has been hit
    int shootBazookaLeftSide(int row);

    //-------------------------------------------------------------------------
    // This function is called if player wants to shoot with bazooka in right direction
    // Function shoots and check if some of other wors or other field have been
    // damaged
    // @param row is the row on which the attack shoult be performed
    int shootBazookaInMiddle(int row, int column);

    //-------------------------------------------------------------------------
    // This function is called if player wants to shoot with bazooka in below worm
    // direction
    // Function shoots and check if some of other wors or other field have been
    // damaged
    // @param col is the column on which the attack shoult be performed
    int shootBazookaRightSide(int row);

    // that has been hit
    // @param target_column is the column coordinate of the first member of the map
    int shootBazookaDown(int column);

    //-------------------------------------------------------------------------
    // This function is called to check wether some of the map members after attack
    // or move should be fall down
    // @param column is the column on which we should apply gravity
    void applyGravity(int column);

    //-------------------------------------------------------------------------
    // This function is called if player wants to shoot with bazooka in upper direction
    // Function shoots and check if some of other wors or other field have been
    // damaged
    // @param column is the column on which the attack shoult be performed
    int shootBazookaTop(int column);

    //-------------------------------------------------------------------------
    // This function find the first member of the map that has been hit by
    // the attack
    // @param direction is the direction in which the attack should be performed
    // @param targer_row is the row coordinate of the first member of the map
    // that has been hit
    // @param target_column is the column coordinate of the first member of the map
    // that has been hit
    // @param worm_in_row tells us the row coordinate of a worm that attacks
    // @param worm_in_col tells us the column coordinate of a worm that attacks
    void findTarget(std::string direction, int &target_row,
                    int &target_column,
                    int worm_in_row, int worm_in_col);


    int shootBazooka(Worm &worm, std::string direction);

    int shootGun(Worm &worm, std::string direction);

    //-------------------------------------------------------------------------
    // This function checks in which direction the gun attack should be executed
    // @param worm is the current worm that is shooting
    // @param direction is the direction in which a shot should be placed
    int shootGunField(int target_row, int target_column);

    //-------------------------------------------------------------------------
    // This function describes how to destroy one field with blowtorch
    // @param target_row is the row coordinate on which the attac should be
    // performed
    // @param target_col ist the column coordinate on which the attack should be
    // performed
    int shootFlameOneField(int target_row, int target_column);

    //-------------------------------------------------------------------------
    // This function describes how to destroy one field
    // @param target_row is the row coordinate on which the attac should be
    // performed
    // @param target_col ist the column coordinate on which the attack should be
    // performed
    int shootBlowtorch(Worm &worm, std::string direction);

    //-------------------------------------------------------------------------
    // This function destroys only one map member
    // @param target_row ist the row coordinate in map where attac should be
    // performed
    // @param target_col is the column coordinate in map where atatck should
    // be performed
    int shootMeleOneField(int target_row, int target_col);

    //-------------------------------------------------------------------------
    // This function performes melee attack
    // @param worm is the current worm that performes the
    bool inField( int row, int column);

    //-------------------------------------------------------------------------
    // This function performes melee attack
    // @param worm is the current worm that performes the
    int actionMelee(Worm &worm);
    //-------------------------------------------------------------------------
    // This function prints all possible commands for user that can be inputed
    void printHelp();

  protected:
    enum Constants
    {
      NUMBER_OF_WEAPONS = 5,
      BAZOOKA = 0,
      TELEPORT = 1,
      BLOWTORCH = 2,
      MELEE = 3,
      AIRSTRIKE = 4,
      NO_WEPON = -1,
      BAZOOKA_DAMAGE = -40,
      GUN_DAMAGE = -25,
      FLAMETHROWER_DAMAGE = -35,
      MELEE_DAMAGE = -50,
      AIRSTRIKE_DAMAGE = -20,
      LEFT = 4,
      UP = 8,
      DOWN = 2,
      RIGHT = 6,
      LEFT_UP = 7,
      RIGHT_UP = 9,
      LEFT_DOWN = 1,
      RIGHT_DOWN = 3,
      SUCCESSFUL_WORM_PLACEMENT = 0,
      DEAD_ID = -1
    };

    int worm_in_row_;  // saves row number in which worm was moved(function move);
    int worm_in_col_; //  saves column number in which worm was moved(function move);

    /// Vector in which map_ is saved;
    std::vector<std::vector<Sep::Field::FieldType>> map_;

    /// After processing map_, final_map is used to keep track
    /// about map after each round.
    std::vector<std::vector<Sep::Field>> final_map_;

  };
}

#endif
