//-----------------------------------------------------------------------------
// Game.cpp
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
// Main calls Game and excutes rounds for game Sworms.
//
//-----------------------------------------------------------------------------
#include "Weapon.h"
#include "Game.h"
#include <iterator>

namespace Sep
{
  const int RETURN_CONTINUE = 1;
  const int RETURN_OK = 0;
  const int RETURN_ERROR_BAD_CONFIG = -1;
  const int RETURN_ERROR_INVALID_TARGET = -2;
  const int RETURN_ERROR_ARGUMENT = -3;
  const std::string ERROR_WRONG_ARGUMENTS = "Usage: ./sworms <config-file>";
  const std::string ERROR_INVALID_TARGET = "[ERROR] invalid target!";
  const std::string ERROR_INVALID_CONFIG_FILE
    = "[ERROR] invalid config file!";
  const std::string END_PLAYER_ONE_WON = "END: Player 1 win!";
  const std::string END_PLAYER_TWO_WON = "END: Player 2 win!";
  const std::string END_DRAW = "END: Draw";
  const std::string COMMAND_PROMPT = "sep> ";
  const int WORM_DEAD = -14;
  const int RETURN_GAME_END_WINNER = -15;
  const int RETURN_GAME_END_DRAW = -16;
  const int RETURN_QUIT = -17;
}


int main(int argc, char* argv[])
{
  int player_on_turn = 1;
  int worm_on_turn_team_1 = 0;
  int worm_on_turn_team_2 = 0;
  int counter_1 = 0;
  int counter_2 = 0;
  int return_value = 0;
  std::string action;
  bool was_move_command = false;

  if(argc != 2)
  {
    std::cout << Sep::ERROR_WRONG_ARGUMENTS << '\n';
    return Sep::RETURN_ERROR_ARGUMENT;
  }

  Sep::Game game;
  if(game.loadConfig(std::string(argv[1])))
    return Sep::RETURN_ERROR_BAD_CONFIG;

  Sep::Worm worm_one;
  Sep::Worm worm_two;
  Sep::Worm worm_three;
  Sep::Worm worm_four;
  Sep::Worm worm_five;
  Sep::Worm worm_six;
  std::vector<Sep::Worm> team_1;
  std::vector<Sep::Worm> team_2;



  game.setupWorms(worm_one, worm_two, worm_three,
                  worm_four, worm_five, worm_six);
  game.placeWorms(worm_one, worm_two, worm_three,
                  worm_four, worm_five, worm_six,
                  team_1, team_2, counter_1, counter_2);
  game.makeMapOfFields();
  game.addPointersOnWorms(team_1);
  game.addPointersOnWorms(team_2);
  game.printFinalMap();

  if(!team_1.empty() && team_2.empty())
  {
    std::cout << Sep::END_PLAYER_ONE_WON << std::endl;
    return  Sep::RETURN_OK;
  }
  if(team_1.empty() && !team_2.empty())
  {
    std::cout << Sep::END_PLAYER_TWO_WON << std::endl;
    return  Sep::RETURN_OK;
  }
  if(team_1.empty() && team_2.empty())
  {
    std::cout << Sep::END_DRAW << std::endl;
    return Sep::RETURN_OK;
  }

  ///Play game
  while(true)
  {
    if(return_value == Sep::RETURN_GAME_END_DRAW)
      return Sep::RETURN_OK;
    if(return_value == Sep::RETURN_GAME_END_WINNER)
      return Sep::RETURN_OK;
    switch (player_on_turn)
    {
      /// Player 1 is playing:
      case 1:
        if(team_1[worm_on_turn_team_1].getWormId() < 1)
        {
          if ((worm_on_turn_team_1 + 1) == static_cast <int>(team_1.size()))
            worm_on_turn_team_1 = 0;
          else
            worm_on_turn_team_1++;
        }
        if(team_1[worm_on_turn_team_1].getWormId() < 1)
        {
          if ((worm_on_turn_team_1 + 1) == static_cast <int>(team_1.size()))
            worm_on_turn_team_1 = 0;
          else
            worm_on_turn_team_1++;
        }

        std::cout << "Player " << team_1[worm_on_turn_team_1].
          getWormTeamNumber()
                  << " Worm "<< team_1[worm_on_turn_team_1].
          getWormName()
                  << " ("
                  << team_1[worm_on_turn_team_1].getWormId()
                  << ")"
                  <<  " at ("<< team_1[worm_on_turn_team_1].
          getWormPlaceRow()
                  << ", " << team_1[worm_on_turn_team_1].
          getWormPlaceCol()
                  << ") ready" << std::endl;
        return_value = Sep::RETURN_CONTINUE;
        while(return_value == Sep::RETURN_CONTINUE
              && return_value != Sep::WORM_DEAD)
        {
          std::cout << Sep::COMMAND_PROMPT;
          getline(std::cin, action);
          return_value = game.actionDecider(action,
                                            team_1[worm_on_turn_team_1],
                                            team_1,
                                            team_2,
                                            was_move_command);
        }
        team_1[worm_on_turn_team_1].setCurrentWeapon("gun");
        if(return_value == Sep::RETURN_QUIT)
          return Sep::RETURN_OK;
        worm_on_turn_team_1++;
        if(worm_on_turn_team_1 == static_cast <int>(team_1.size()))
        {
          worm_on_turn_team_1 = 0;
        }
        was_move_command = false;
        break;

        ///Player 2 is playing:
      case 2:
        if(team_2[worm_on_turn_team_2].getWormId() < 1)
        {
          if ((worm_on_turn_team_2 + 1) == static_cast <int>(team_2.size()))
            worm_on_turn_team_2 = 0;
          else
            worm_on_turn_team_2++;
        }
        if(team_2[worm_on_turn_team_2].getWormId() < 1)
        {
          if ((worm_on_turn_team_2 + 1) == static_cast <int>(team_2.size()))
            worm_on_turn_team_2 = 0;
          else
            worm_on_turn_team_2++;
        }
        std::cout << "Player " << team_2[worm_on_turn_team_2].
          getWormTeamNumber()
                  << " Worm "<< team_2[worm_on_turn_team_2].
          getWormName()
                  << " (" << team_2[worm_on_turn_team_2].getWormId() << ")"
                  <<" at ("<< team_2[worm_on_turn_team_2].
          getWormPlaceRow()
                  << ", " << team_2[worm_on_turn_team_2].
          getWormPlaceCol()
                  << ") ready" << std::endl;
        return_value = Sep::RETURN_CONTINUE;
        while(return_value == Sep::RETURN_CONTINUE
              && return_value != Sep::WORM_DEAD)
        {
          std::cout << "sep> ";
          getline(std::cin, action);
          return_value = game.actionDecider(action,
                                            team_2[worm_on_turn_team_2],
                                            team_1,
                                            team_2,
                                            was_move_command);
        }
        team_2[worm_on_turn_team_2].setCurrentWeapon("gun");
        if(return_value == Sep::RETURN_QUIT)
          return Sep::RETURN_OK;
        worm_on_turn_team_2++;
        if(worm_on_turn_team_2 == static_cast <int>(team_2.size()))
        {
          worm_on_turn_team_2 = 0;
        }
        was_move_command = false;
        break;
    }

    if(player_on_turn == 1)
      player_on_turn = 2;
    else
      player_on_turn = 1;
    if(return_value != Sep::RETURN_GAME_END_WINNER
       && return_value != Sep::RETURN_GAME_END_DRAW)
      game.printFinalMap();
  }
}
