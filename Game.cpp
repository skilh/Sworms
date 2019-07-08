
//-----------------------------------------------------------------------------
// Game.cpp
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
// Game describes logic of sworms game.
// There are two teams playing against each other (Team1 and Team2).
// The winner is the team who first eliminates worms of another team.
// Using STDOUT/STDIN user/s can control team/s with help of some basic commands.
//
// For more detailed description of commands type: "help" on STDIN.
// Enjoy and have fun :-)
//-----------------------------------------------------------------------------
//
#include "Game.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>

const std::string Sep::Game::ERROR_INVALID_TARGET
  = "[ERROR] invalid target!";
const std::string Sep::Game::ERROR_INVALID_CONFIG_FILE
  = "[ERROR] invalid config file!";
const std::string Sep::Game::ERROR_NO_AMMUNITION
  = "[ERROR] no ammunition\n";
const std::string Sep::Game::ERROR_INVALID_PARAMETER
  = "[ERROR] invalid parameter!\n";
const std::string Sep::Game::ERROR_WRONG_PARAMETAR_COUNT
  = "[ERROR] wrong parameter count!";
const std::string Sep::Game::ERROR_UNKNOWN_COMMAND
  = "[ERROR] unknown command!";
const std::string Sep::Game::ERROR_COMMAND_NOT_ALLOWED
  = "[ERROR] command currently not allowed!";


const std::string Sep::Game::END_PLAYER_ONE_WON
  = "END: Player 1 win!";
const std::string Sep::Game::END_PLAYER_TWO_WON
  = "END: Player 2 win!";
const std::string Sep::Game::END_DRAW
  = "END: Draw";

const std::string Sep::Game::WORM_PICKED_UP_BAZOOKA_MESSAGE
  = ") picked up 1 of bazooka\n";
const std::string Sep::Game::WORM_PICKED_UP_TELEPORT_MESSAGE
  = ") picked up 1 of teleport\n";
const std::string Sep::Game::WORM_PICKED_UP_BLOWTORCH_MESSAGE
  = ") picked up 1 of blowtorch\n";
const std::string Sep::Game::WORM_PICKED_UP_MELEE_MESSAGE
  = ") picked up 1 of melee\n";
const std::string Sep::Game::WORM_PICKED_UP_AIRSTRIKE_MESSAGE
  = ") picked up 1 of airstrike\n";
const std::string Sep::Game::DROWNED_MESSAGE
  = ") drowned.\n";
const std::string Sep::Game::FELL_INTO_HIS_DEATH
  = ") fell into his death\n";
const std::string Sep::Game::AMMUNITION_STATE
  = " (ammunition: ";

const std::string Sep::Game::MESSAGE_SHOT_HIT_CHEST
  = "Shot hit Chest at position (";
const std::string Sep::Game::MESSAGE_SHOT_HIT_EARTH
  = "Shot hit Earth at position (";
const std::string Sep::Game::MESSAGE_SHOT_HIT_WORM
  = "Shot hit Worm at position (";
const std::string Sep::Game::MESSAGE_WORM_DIED
  = ") died.";
const std::string Sep::Game::MESSAGE_SHOT_MISSED
  = "Shot missed...\n";

const std::string Sep::Game::MESSAGE_CANT_MOVE
  = "[WARNING] can't move further";


Sep::Field field;


void Sep::Game::setBoardHeight(int y_coordinate)
{
  board_height_ = y_coordinate;
}

void Sep::Game::setBoardWidth(int x_coordinate)
{
  board_width_ = x_coordinate;
}

void Sep::Game::setWormAtRow(int row)
{
  worm_in_row_ = row;
}


int Sep::Game::wormIsAtRow(int row)
{
  return row;
}

int Sep::Game::wormIsAtCol(int col)
{
  return col;
}

void Sep::Game::setWorAtCol(int col)
{
  worm_in_col_ = col;
}

Sep::Game::Game() : board_width_(0), board_height_(0), map_(80),
  final_map_(80) {};


bool hasOnlySpaces(std::string &str)
{
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
  {
    if (*it != ' ')
      return false;
  }
  return true;
}


std::string deleteWhiteSpaceBeforeFirstLetter(std::string str)
{
  int sentence_iterator = 0;
  int temp_var = 0;
  if(str[sentence_iterator] == ' ')
  {
    while (str[sentence_iterator] == ' ')
    {
      sentence_iterator++;
      temp_var = sentence_iterator;
    }
  }
  str.erase(str.begin(), str.begin() + temp_var);
  return str;
}


bool isLineComment(std::string str)
{
  if(str.find("#") == 0)
    return true;
  else
    return false;
}


int countWordsInSentence(std::string str)
{
  bool in_spaces = true;
  int num_words = 0;
  int sentence_iterator = 0;

  while (str[sentence_iterator] != '\0')
  {
    if (std::isspace(str[sentence_iterator]))
    {
      in_spaces = true;
    }
    else if (in_spaces)
    {
      num_words++;
      in_spaces = false;
    }
    ++sentence_iterator;
  }
  return num_words;
}


int Sep::Game::removeDupWord(std::string str)
{
  // Used to split string around spaces.
  std::istringstream ss(str);
  int temp_number;
  int count_words = 0;
  std::string word;
  // Traverse through all words
  do
  {
    count_words++;
    // Read temp_number word
    ss >> word;
    if(count_words == 2 || count_words == 3)
    {
      std::istringstream iss(word);
      iss >> temp_number;
      if (!iss.good())
      {
        //something happened
      }
      if(temp_number < 10 || temp_number > 80)
      {
        std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
        return RETURN_CONTINUE;
      }

      if(count_words == 2)
      {
        setBoardWidth(temp_number);
      }
      if(count_words == 3)
      {
        setBoardHeight(temp_number);
      }
    }
    // While there is more to read
  } while (ss);
  return RETURN_OK;
}


void Sep::Game::setMap(std::vector<std::string> map)
{
  int height = getBoardHeight();
  int map_iterator = 0;
  int vector_iterator = 0;

  for(auto &ite : map)
  {
    for(vector_iterator = 0;
        vector_iterator < static_cast<int>(ite.length()); vector_iterator++)
    {
      map_[map_iterator].push_back(field.getEnum(ite[vector_iterator]));
    }
    map_iterator++;
  }

  std::vector< std::vector<Sep::Field::FieldType> >::iterator row; // iterates through rows
  std::vector<Sep::Field::FieldType >::iterator col; // iterates through columns
  for (row = map_.begin(); row != (map_.begin() + height); row++)
  {
    for(col = row->begin(); col != row->end(); col++)
    {
      field.setType_(*col);
    }
  }
}


void Sep::Game::printMap()
{
  int width = getBoardWidth();
  int height = getBoardHeight();
  int column_iterator = 0;
  int row_iterator = 0;

  std::string first_and_last_line( static_cast<unsigned long>(width), 'A');
  for(column_iterator = 0; column_iterator < (width); column_iterator++)
  {
    if(column_iterator == width)
      break;
    else
    {
      if(row_iterator % 10 == 0)
        row_iterator = 0;
      first_and_last_line[column_iterator] += (char)row_iterator - 17;
    }
    row_iterator++;
  }
  column_iterator = 0;
  std::cout << "Current Map:" << std::endl;
  std::cout << std::setw(width + 2);
  std::cout << first_and_last_line << std::endl;

  std::vector< std::vector<Sep::Field::FieldType> >::iterator row;
  std::vector<Sep::Field::FieldType >::iterator col;
  for (row = map_.begin(); row != (map_.begin() + height); row++)
  {
    std::cout << std::setw(2);
    std::cout << column_iterator;
    for(col = row->begin(); col != row->end(); col++)
    {
      std::cout << field.getCharacter(*col);
    }
    std::cout << std::setw(2);
    std::cout << column_iterator << std::endl;
    column_iterator++;
  }
  std::cout << std::setw(width + 2);
  std::cout << first_and_last_line << std::endl;
}


int Sep::Game::loadConfig(std::string cfg_file_)
{
  std::vector<std::string> probe_for_map ;
  std::string content;
  std::ifstream myfile;
  myfile.open(cfg_file_);
  bool is_space_ = true; // 1 if empty line occures, otherwise 0;
  int row_iterator = 0;
  bool must_be_acctual_map = false;
  int size_return_value = 0;
  int counte_word_size = 0;
  int count_word_map = 0;
  int counter_height = 0;
  bool is_map = false;
  bool is_size = false;
  int width;
  int height;
  int loop_counter1;
  int loop_counter2;
  std::string compare_line;

  if(!myfile)
  {
    std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
    return RETURN_CONTINUE;
  }

  /// Save content of file
  while (std::getline(myfile, content))
  {
    probe_for_map.push_back(content);
  }
  myfile.close();

  for(auto &ite : probe_for_map)
  {
    is_space_ = hasOnlySpaces(ite);
    /// Check if first line is magic word;
    row_iterator++;
    if(row_iterator == 1)
    {
      std::string check_if_start_is_good =
        deleteWhiteSpaceBeforeFirstLetter(ite);
      if(check_if_start_is_good != "#SWORMS")
      {
        std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
        return RETURN_ERROR_INVALID_CONFIG;
      }
    }
    /// Delete all white space lines
    if(is_space_ == 1)
    {
      std::string temp = ite;
      std::vector<std::string>::iterator place =
        std::find(probe_for_map.begin(), probe_for_map.end(), temp);
      if(probe_for_map.end() != place)
        probe_for_map.erase(place);
    }
  }

  std::vector<std::string> temp_vector;
  for(auto &ite : probe_for_map)
  {
    ite = deleteWhiteSpaceBeforeFirstLetter(ite);
    bool is_comment = isLineComment(ite);

    ///Here check line which doesn't start with comment/'#';
    if(is_comment != 1)
    {
      if(ite.find("SIZE: ") == 0)
      {
        is_size = true;
        counte_word_size++;
        int num_of_words = countWordsInSentence(ite);
        if(num_of_words != 3)
        {
          std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
          return RETURN_ERROR_INVALID_CONFIG;
        }
        size_return_value = removeDupWord(ite);
        if(size_return_value == 1)
          return RETURN_ERROR_INVALID_CONFIG;
        else
          continue;
      }
      /// Here check if line marks start of map;
      if(ite.find("MAP: ") == 0 || ite.find("MAP:") == 0)
      {
        is_map = true;
        count_word_map++;
        int num_of_words = countWordsInSentence(ite);
        if(num_of_words != 1)
        {
          std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
          return RETURN_ERROR_INVALID_CONFIG;
        }
        must_be_acctual_map = true;
      }
      else if(must_be_acctual_map == true)
      {
        temp_vector.push_back(ite);
      }
    }
  }
  if(counte_word_size != 1 || count_word_map != 1)
  {
    std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
    return RETURN_ERROR_INVALID_CONFIG;
  }
  width = getBoardWidth();
  height = getBoardHeight();

  for(auto &ite : temp_vector)
  {
    counter_height++;
    if(width != static_cast<int>(ite.length()))
    {
      std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
      return RETURN_ERROR_INVALID_CONFIG;
    }
    loop_counter1 = 0;
    while(loop_counter1 < width)
    {
      if(ite[loop_counter1] != 'A' && ite[loop_counter1] != 'W' &&
        ite[loop_counter1] != 'E')
      {
        std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
        return RETURN_ERROR_INVALID_CONFIG;
      }
      loop_counter1++;
    }
  }

  if(counter_height != height)
  {
    std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
    return RETURN_ERROR_INVALID_CONFIG;
  }
  std::string first_row = *temp_vector.begin();
  std::string last_row = *(temp_vector.end() - 1);

  /// Only Air is allowed to be in first row
  std::string first_row_compare(static_cast<unsigned long>(width), 'A');
  if(first_row != first_row_compare)
  {
    std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
    return RETURN_ERROR_INVALID_CONFIG;
  }
  loop_counter2 = 0;

  /// Last line must have Air or Earth, no Air is allowed here;
  while (loop_counter2 < width)
  {
    if(last_row[loop_counter2] != 'E' && last_row[loop_counter2] != 'W')
    {
      std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
      return RETURN_ERROR_INVALID_CONFIG;
    }
    loop_counter2++;
  }
  ///Water can be only in last row;
  loop_counter2 = 0;
  for(auto &ite : temp_vector)
  {
    compare_line = ite;
    if(compare_line == *(temp_vector.end() - 1))
      break;
    while(loop_counter2 <= static_cast <int> (compare_line.length()))
    {
      if(compare_line[loop_counter2] == 'W')
      {
        std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
        return RETURN_ERROR_INVALID_CONFIG;
      }
      loop_counter2++;
    }
    loop_counter2 = 0;
  }

  setMap(temp_vector);

  /// When there is no Map or Size line;
  if(is_map == false || is_size == false)
  {
    std::cout << ERROR_INVALID_CONFIG_FILE << std::endl;
    return RETURN_ERROR_INVALID_CONFIG;
  }

  return RETURN_OK;
}


void Sep::Game::setupWorms(Worm &worm_one, Worm &worm_two,
                           Worm &worm_three,
                           Worm &worm_four, Worm &worm_five,
                           Worm &worm_six)
{
  int width = getBoardWidth();
  worm_one.setWormId(1);
  worm_one.setWormName(random.getWormName());
  worm_one.setWormPlaceCol(random.getRandomInt(0, width - 1));
  worm_one.order_ = 1;
  worm_one.setWormInTeam(1);

  worm_two.setWormId(2);
  worm_two.setWormName(random.getWormName());
  worm_two.setWormPlaceCol(random.getRandomInt(0, width - 1));
  worm_two.order_ = 3;
  worm_two.setWormInTeam(1);

  worm_three.setWormId(3);
  worm_three.setWormName(random.getWormName());
  worm_three.setWormPlaceCol(random.getRandomInt(0, width - 1));
  worm_three.order_ = 5;
  worm_three.setWormInTeam(1);

  worm_four.setWormId(4);
  worm_four.setWormName(random.getWormName());
  worm_four.setWormPlaceCol(random.getRandomInt(0, width - 1));
  worm_four.order_ = 2;
  worm_four.setWormInTeam(2);

  worm_five.setWormId(5);
  worm_five.setWormName(random.getWormName());
  worm_five.setWormPlaceCol(random.getRandomInt(0, width - 1));
  worm_five.order_ = 4;
  worm_five.setWormInTeam(2);

  worm_six.setWormId(6);
  worm_six.setWormName(random.getWormName());
  worm_six.setWormPlaceCol(random.getRandomInt(0, width - 1));
  worm_six.order_ = 6;
  worm_six.setWormInTeam(2);


}

int Sep::Game::checkForCasualities(std::vector<Sep::Worm> &team_1,
                                   std::vector<Sep::Worm> &team_2,
                                   Worm &worm_in_game,
                                   int &number_of_worms_1,
                                   int &number_of_worms_2)
{
  int counter_player_1 = 0;
  int counter_player_2 = 0;
  number_of_worms_1 = 0;
  number_of_worms_2 = 0;
  int iterator = 0;

  for( iterator = 0; iterator < static_cast <int> (team_1.size()); iterator++)
  {
    if(team_1[iterator].getWormId() < 1)
      counter_player_1++;
    else
      number_of_worms_1++;
  }

  for( iterator = 0; iterator < static_cast <int> (team_2.size());
       iterator++)
  {
    if(team_2[iterator].getWormId() < 1)
      counter_player_2++;
    else
      number_of_worms_2++;
  }



  if(number_of_worms_1 > 0 && number_of_worms_2 < 1)
  {
    printFinalMap();
    std::cout<< END_PLAYER_ONE_WON << std::endl;
    return RETURN_INFO_END_WINNER;
  }
  if(number_of_worms_2 > 0 && number_of_worms_1 < 1)
  {
    printFinalMap();
    std::cout<< END_PLAYER_TWO_WON << std::endl;
    return RETURN_INFO_END_WINNER;
  }
  if(number_of_worms_1 < 1 && number_of_worms_2 < 1)
  {
    printFinalMap();
    std::cout<< END_DRAW << std::endl;
    return RETURN_INFO_END_DRAW;
  }

  if(worm_in_game.getWormId() < 1)
  {
    return RETURN_INFO_WORM_DEAD;
  }
  else
    return RETURN_OK;

}


void Sep::Game::placeWorms(Worm &worm_one,
                           Worm &worm_two,
                           Worm &worm_three,
                           Worm &worm_four,
                           Worm &worm_five,
                           Worm &worm_six,
                           std::vector<Sep::Worm> &player1_worms,
                           std::vector<Sep::Worm> &player2_worms,
                           int &player_one_counter,
                           int &player_two_counter)
{
  int signal_1 = 0;
  int signal_2 = 0;
  int signal_3 = 0;
  int signal_4 = 0;
  int signal_5 = 0;
  int signal_6 = 0;
  int loop_iterator = 0;

  while(loop_iterator != 1)
  {
    if(addWorm(worm_one) != SUCCESSFUL_WORM_PLACEMENT)
    {
      worm_one.setWormId(DEAD_ID);
      signal_1 = 1;
    }
    if(addWorm(worm_two) != SUCCESSFUL_WORM_PLACEMENT)
    {
      worm_two.setWormId(DEAD_ID);
      signal_2 = 1;
    }
    if(addWorm(worm_three)!= SUCCESSFUL_WORM_PLACEMENT)
    {
      worm_three.setWormId(DEAD_ID);
      signal_3 = 1;
    }
    if(addWorm(worm_four) != SUCCESSFUL_WORM_PLACEMENT)
    {
      worm_four.setWormId(DEAD_ID);
      signal_4 = 1;
    }
    if(addWorm(worm_five) != SUCCESSFUL_WORM_PLACEMENT)
    {
      worm_five.setWormId(DEAD_ID);
      signal_5 = 1;
    }
    if(addWorm(worm_six) != SUCCESSFUL_WORM_PLACEMENT)
    {
      worm_six.setWormId(DEAD_ID);
      signal_6 = 1;
    }
    loop_iterator = 1;
  }
  if(signal_1 == SUCCESSFUL_WORM_PLACEMENT)
  {
    player1_worms.push_back(worm_one);
    player_one_counter++;
  }
  if(signal_4 == SUCCESSFUL_WORM_PLACEMENT)
  {
    player2_worms.push_back(worm_four);
    player_two_counter++;
  }
  if(signal_2 == SUCCESSFUL_WORM_PLACEMENT)
  {
    player1_worms.push_back(worm_two);
    player_one_counter++;
  }
  if(signal_5 == SUCCESSFUL_WORM_PLACEMENT)
  {
    player2_worms.push_back(worm_five);
    player_two_counter++;
  }
  if(signal_3 == SUCCESSFUL_WORM_PLACEMENT)
  {
    player1_worms.push_back(worm_three);
    player_one_counter++;
  }
  if(signal_6 == SUCCESSFUL_WORM_PLACEMENT)
  {
    player2_worms.push_back(worm_six);
    player_two_counter++;
  }
}


int Sep::Game::addWorm(Worm &worm_to_be_added)
{
  int height = getBoardHeight();
  int width = getBoardWidth();
  int row = worm_to_be_added.getWormPlaceRow();
  int col = worm_to_be_added.getWormPlaceCol();
  std::cout << "spawning " << worm_to_be_added.getWormName()
            << " (" << worm_to_be_added.getWormId() << ") "
            << "at (" << row <<", " << col << ")\n";

  if(row > height || col > width)
  {
    std::cout << ERROR_INVALID_TARGET << std::endl;
    return RETURN_ERROR_INVALID_TARGET;
  }

  Field::FieldType temp_var_want = map_[row][col];
  if(temp_var_want == field.AIR)
  {
    while(map_[row + 1][col] == field.AIR)
      row++;
    if(map_[row + 1][col] == field.WATER)
    {
      std::cout << worm_to_be_added.getWormName() << " ("
                << worm_to_be_added.getWormId()
                << DROWNED_MESSAGE;
      return RETURN_DIED_AT_BEGINN;
    }
    else if(map_[row + 1][col] == field.WORM || map_[row + 1]
                                                [col] == field.EARTH
            || map_[row + 1][col] == field.WORM2)
    {
      if(worm_to_be_added.getWormId() <= 3)
      {
        field.setType_(map_[row][col] = field.WORM);
        /// Save place where function placed worm since it can be used in next step/s;
        setWormAtRow(row);
        setWorAtCol(col);
        worm_to_be_added.setWormPlaceRow(wormIsAtRow(worm_in_row_));
        worm_to_be_added.setWormPlaceCol(wormIsAtCol(worm_in_col_));
      }
      if(worm_to_be_added.getWormId() > 3)
      {
        field.setType_(map_[row][col] = field.WORM2);
        /// Save place where function placed worm since it can be used in next step/s;
        setWormAtRow(row);
        setWorAtCol(col);
        worm_to_be_added.setWormPlaceRow(wormIsAtRow(worm_in_row_));
        worm_to_be_added.setWormPlaceCol(wormIsAtCol(worm_in_col_));
      }
    }
  }
  return RETURN_OK;
}

int Sep::Game::moveWorm(Worm &worm, int direction)
{
  int row = worm.getWormPlaceRow();
  int col = worm.getWormPlaceCol();
  int deducted_hp;
  int old_col;
  int old_row = row;

  if(direction < 0)
    old_col = col + 1;
  if(direction > 0)
    old_col = col - 1;

  Field temp_object = final_map_[row][col];

  if(temp_object.getType() == field.AIR)
  {
    while(final_map_[row][col].getType() == field.AIR
          || final_map_[row][col].getType() == field.CHEST)
    {
      row++;
      if(final_map_[row][col].getType() == field.CHEST)
      {
        if(final_map_[row][col].weapon_in_chest_ == BAZOOKA)
        {
          worm.worm_weapon_bazooka_.setCurrentAmmunition(1);
          std::cout<< worm.getWormName() << " ("
                   << worm.getWormId()
                   << WORM_PICKED_UP_BAZOOKA_MESSAGE;
        }
        else if(final_map_[row][col].weapon_in_chest_ == TELEPORT)
        {
          worm.worm_weapon_tepelport_.setCurrentAmmunition(1);
          std::cout<< worm.getWormName()<<" ("
                   << worm.getWormId()
                   << WORM_PICKED_UP_TELEPORT_MESSAGE;
        }
        else if(final_map_[row][col].weapon_in_chest_ == BLOWTORCH)
        {
          worm.worm_weapon_flamethrower_.setCurrentAmmunition(1);
          std::cout<< worm.getWormName() <<" ("
                   << worm.getWormId()
                   << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
        }
        else if(final_map_[row][col].weapon_in_chest_ == MELEE)
        {
          worm.worm_weapon_melee_.setCurrentAmmunition(1);
          std::cout<< worm.getWormName() <<" ("
                   << worm.getWormId()
                   << WORM_PICKED_UP_MELEE_MESSAGE;
        }
        else if(final_map_[row][col].weapon_in_chest_ == AIRSTRIKE)
        {
          worm.worm_weapon_airstrike_.setCurrentAmmunition(1);
          std::cout<< worm.getWormName()<<" ("
                   << worm.getWormId()
                   << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
        }
        final_map_[row][col].setType_(field.AIR);
        final_map_[row][col].weapon_in_chest_ = NO_WEPON;
      }
    }

    deducted_hp = (row - old_row - 2) * 10;
    if(final_map_[row][col].getType() == field.WATER)
    {
      std::cout << worm.getWormName()
               <<" (" << worm.getWormId()<< DROWNED_MESSAGE;
      worm.setWormHp(-(worm.getWormHp()));
      worm.setWormId(DEAD_ID);
      return RETURN_INFO_WORM_DROWNED;
    }
    else if(final_map_[row][col].getType() == field.WORM ||
            final_map_[row][col].getType() == field.WORM2||
            final_map_[row][col].getType() == field.EARTH)
    {
      if(deducted_hp > worm.getWormHp())
      {
        std::cout<< worm.getWormName()
                 << " (" << worm.getWormId()
                 << ") fell into his death\n";
        worm.setWormHp(-worm.getWormHp());
        worm.setWormId(DEAD_ID);
        return RETURN_INFO_WORM_DEAD;
      }
      else
      {
        if(deducted_hp > 0)
        {
          std::cout<< worm.getWormName()<<" ("
                   << worm.getWormId()<<") took "
                   << deducted_hp<<"hp damage\n";
          worm.setWormHp(-deducted_hp);
          final_map_[row - 1][col].setType_
            (final_map_[old_row][old_col].getType());
          final_map_[row - 1][col]. pointer_to_worm_ =
            final_map_[old_row][old_col]. pointer_to_worm_;
          worm.setWormPlaceCol(col);
          worm.setWormPlaceRow(row - 1);
          return RETURN_OK;
        }
        else
        {
          final_map_[row - 1][col].setType_
            (final_map_[old_row][old_col].getType());
          final_map_[row - 1][col]. pointer_to_worm_ =
            final_map_[old_row][old_col]. pointer_to_worm_;
          worm.setWormPlaceCol(col);
          worm.setWormPlaceRow(row - 1);
          return RETURN_OK;
        }
      }
    }
  }
  else if(temp_object.getType() == field.CHEST)
  {
    if(final_map_[row][col].weapon_in_chest_ == BAZOOKA)
    {
      worm.worm_weapon_bazooka_.setCurrentAmmunition(1);
      std::cout << worm.getWormName()<< " (" << worm.getWormId()
               << WORM_PICKED_UP_BAZOOKA_MESSAGE;
    }
    else if(final_map_[row][col].weapon_in_chest_ == TELEPORT)
    {
      worm.worm_weapon_tepelport_.setCurrentAmmunition(1);
      std::cout << worm.getWormName()<< " ("  << worm.getWormId()
               << WORM_PICKED_UP_TELEPORT_MESSAGE;
    }
    else if(final_map_[row][col].weapon_in_chest_ == BLOWTORCH)
    {
      worm.worm_weapon_flamethrower_.setCurrentAmmunition(1);
      std::cout << worm.getWormName() << " (" << worm.getWormId()
               << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
    }
    else if(final_map_[row][col].weapon_in_chest_ == AIRSTRIKE)
    {
      worm.worm_weapon_airstrike_.setCurrentAmmunition(1);
      std::cout << worm.getWormName() << " (" << worm.getWormId()
               << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
    }
    else if(final_map_[row][col].weapon_in_chest_ == MELEE)
    {
      worm.worm_weapon_melee_.setCurrentAmmunition(1);
      std::cout << worm.getWormName() << " ("  << worm.
        getWormId()
               << WORM_PICKED_UP_MELEE_MESSAGE;
    }
    final_map_[row][col].setType_(field.AIR);
    final_map_[row][col].weapon_in_chest_ = NO_WEPON;

    while(final_map_[row][col].getType() == field.AIR
          || final_map_[row][col].getType() == field.CHEST)
    {
      row++;
      if(final_map_[row][col].getType() == field.CHEST)
      {
        if(final_map_[row][col].weapon_in_chest_ == BAZOOKA)
        {
          worm.worm_weapon_bazooka_.setCurrentAmmunition(1);
          std::cout << worm.getWormName() << " ("  << worm.
            getWormId()
                   << WORM_PICKED_UP_BAZOOKA_MESSAGE;
        }
        else if(final_map_[row][col].weapon_in_chest_ == TELEPORT)
        {
          worm.worm_weapon_tepelport_.setCurrentAmmunition(1);
          std::cout << worm.getWormName() <<" ("  << worm.
            getWormId()
                   << WORM_PICKED_UP_TELEPORT_MESSAGE;
        }
        else if(final_map_[row][col].weapon_in_chest_ == BLOWTORCH)
        {
          worm.worm_weapon_flamethrower_.setCurrentAmmunition(1);
          std::cout << worm.getWormName() << " ("  << worm.
            getWormId()
                   << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
        }
        else if(final_map_[row][col].weapon_in_chest_ == AIRSTRIKE)
        {
          worm.worm_weapon_airstrike_.setCurrentAmmunition(1);
          std::cout << worm.getWormName() << " (" << worm.
            getWormId()
                   << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
        }
        else if(final_map_[row][col].weapon_in_chest_ == MELEE)
        {
          worm.worm_weapon_melee_.setCurrentAmmunition(1);
          std::cout << worm.getWormName() << " (" << worm.
            getWormId()
                   << WORM_PICKED_UP_MELEE_MESSAGE;
        }
        final_map_[row][col].setType_(field.AIR);
        final_map_[row][col].weapon_in_chest_ = NO_WEPON;
      }
    }
    deducted_hp = (row - old_row - 2) * 10;
    if(final_map_[row][col].getType() == field.WATER)
    {
      std::cout<< worm.getWormName() << " ("
               << worm.getWormId()
               << DROWNED_MESSAGE;
      worm.setWormHp(- 100);
      worm.setWormId(DEAD_ID);
      return RETURN_INFO_WORM_DROWNED;
    }
    else if(final_map_[row][col].getType() == field.WORM ||
            final_map_[row][col].getType() == field.WORM2||
            final_map_[row][col].getType() == field.EARTH)
    {
      if(deducted_hp > worm.getWormHp())
      {
        std::cout << worm.getWormName()<<" (" << worm.
          getWormId()
                 <<") fell into his death\n";
        worm.setWormHp(-worm.getWormHp());
        worm.setWormId(DEAD_ID);
        return RETURN_INFO_WORM_DEAD;
      }
      else
      {
        if(deducted_hp > 0)
        {
          std::cout << worm.getWormName()<<" (" << worm.
            getWormId()
                   <<") took "<< deducted_hp<<"hp damage\n";
          worm.setWormHp(-deducted_hp);
          final_map_[row - 1][col].setType_
            (final_map_[old_row][old_col].getType());
          final_map_[row - 1][col]. pointer_to_worm_ =
            final_map_[old_row][old_col]. pointer_to_worm_;
          worm.setWormPlaceCol(col);
          worm.setWormPlaceRow(row - 1);
          return RETURN_OK;
        }
        else
        {
          final_map_[row - 1][col].setType_
            (final_map_[old_row][old_col].getType());
          final_map_[row - 1][col]. pointer_to_worm_ =
            final_map_[old_row][old_col]. pointer_to_worm_;
          worm.setWormPlaceCol(col);
          worm.setWormPlaceRow(row - 1);
          return RETURN_OK;
        }
      }
    }
  }
  return RETURN_OK;
}

void Sep::Game::choose(std::string weapon, Worm &worm)
{
  std::string was_current = worm.getCurrentWeapon();
  worm.setCurrentWeapon(weapon);

  if(worm.getCurrentWeapon() == worm.worm_weapon_gun_
                                         .getWeaponName())
  {
    std::cout << "Chose weapon " << worm.getCurrentWeapon()
             << " Ammunition: inf\n";
  }
  else if(worm.getCurrentWeapon() == worm.
    worm_weapon_bazooka_.getWeaponName())
  {
    if(worm.worm_weapon_bazooka_.getCurrentAmmunition() > 0)
    {
      std::cout<< "Chose weapon " << worm.getCurrentWeapon()
               << " Ammunition: "
               << worm.worm_weapon_bazooka_.getCurrentAmmunition()
               <<"\n";
    }
    else
    {
      worm.setCurrentWeapon(was_current);
      std::cout << ERROR_NO_AMMUNITION;
    }
  }
  else if(worm.getCurrentWeapon() == worm.
    worm_weapon_airstrike_.getWeaponName())
  {
    if(worm.worm_weapon_airstrike_.getCurrentAmmunition() > 0)
    {
      std::cout<<"Chose weapon " << worm.getCurrentWeapon()
               <<" Ammunition: "
                << worm.worm_weapon_airstrike_.getCurrentAmmunition()
               <<"\n";
    }
    else
    {
      worm.setCurrentWeapon(was_current);
      std::cout << ERROR_NO_AMMUNITION;
    }
  }
  else if(worm.getCurrentWeapon() == worm.
    worm_weapon_flamethrower_.getWeaponName())
  {
    if(worm.worm_weapon_flamethrower_.getCurrentAmmunition() > 0)
    {
      std::cout<<"Chose weapon " << worm.getCurrentWeapon()
               <<" Ammunition: "
                << worm.worm_weapon_flamethrower_.
                 getCurrentAmmunition()<<"\n";
    }
    else
    {
      worm.setCurrentWeapon(was_current);
      std::cout << ERROR_NO_AMMUNITION;
    }
  }
  else if(worm.getCurrentWeapon() == worm.
    worm_weapon_melee_.getWeaponName())
  {
    if(worm.worm_weapon_melee_.getCurrentAmmunition() > 0)
    {
      std::cout<<"Chose weapon " << worm.getCurrentWeapon()
               <<" Ammunition: "
                << worm.worm_weapon_melee_.getCurrentAmmunition()
               <<"\n";
    }
    else
    {
      worm.setCurrentWeapon(was_current);
      std::cout << ERROR_NO_AMMUNITION;
    }
  }
  else if(worm.getCurrentWeapon() ==
    worm.worm_weapon_tepelport_.getWeaponName())
  {
    if(worm.worm_weapon_tepelport_.getCurrentAmmunition() > 0)
    {
      std::cout<<"Chose weapon " << worm.getCurrentWeapon()
               <<" Ammunition: "
                << worm.worm_weapon_tepelport_.
                 getCurrentAmmunition() << "\n";
    }
    else
    {
      worm.setCurrentWeapon(was_current);
      std::cout << ERROR_NO_AMMUNITION;
    }
  }
  else
  {
    worm.setCurrentWeapon(was_current);
    std::cout << ERROR_INVALID_PARAMETER;
  }
}

void Sep::Game::state(Worm worm, std::vector<Sep::Worm> worms_team_one,
                      std::vector<Sep::Worm> worms_team_two)
{
  std::vector<std::string> weapons;

  std::cout<<"current player: " << worm.getWormTeamNumber()<<"\n";
  std::cout<<"current worm: " << worm.getWormName()<<" ("
            << worm.getWormId()<<") \n";
  std::cout<<"\n";

  weapons.push_back(worm.worm_weapon_gun_.getWeaponName());
  weapons.push_back(worm.worm_weapon_bazooka_.getWeaponName());
  weapons.push_back(worm.worm_weapon_tepelport_.getWeaponName());
  weapons.push_back(worm.worm_weapon_flamethrower_.getWeaponName());
  weapons.push_back(worm.worm_weapon_melee_.getWeaponName());
  weapons.push_back(worm.worm_weapon_airstrike_.getWeaponName());

  std::cout<<"weapons of current worm:\n";
  for(int iterator = 0; iterator < static_cast <int> (weapons.size());
   iterator++)
  {
    if(worm.worm_weapon_gun_.getWeaponName() == weapons[iterator])
    {
      if(worm.worm_weapon_gun_.getWeaponName() == worm.getCurrentWeapon())
      {
        std::cout<<"  " << worm.worm_weapon_gun_.getWeaponName()
                 <<" (ammunition: inf) *\n";
      }
      else
      {
        std::cout<<"  " << worm.worm_weapon_gun_.getWeaponName()
                 <<" (ammunition: inf)\n";
      }
    }
    else if(worm.worm_weapon_bazooka_.getWeaponName() == weapons[iterator] &&
            worm.worm_weapon_bazooka_.getCurrentAmmunition() > 0)
    {
      if(worm.worm_weapon_bazooka_.getWeaponName() == worm.getCurrentWeapon())
      {
        std::cout << "  " << worm.worm_weapon_bazooka_.getWeaponName()
                  << AMMUNITION_STATE
                  << worm.worm_weapon_bazooka_.getCurrentAmmunition()
                  << ") *\n";
      }
      else
      {
        std::cout << "  " << worm.worm_weapon_bazooka_.getWeaponName()
                  << AMMUNITION_STATE
                  << worm.worm_weapon_bazooka_.getCurrentAmmunition()
                  << ")\n";
      }
    }
    else if(worm.worm_weapon_tepelport_.getWeaponName() == weapons[iterator]
            && worm.worm_weapon_tepelport_.getCurrentAmmunition() > 0)
    {
      if(worm.worm_weapon_tepelport_.getWeaponName() ==
       worm.getCurrentWeapon())
      {
        std::cout << "  " << worm.worm_weapon_tepelport_.getWeaponName()
                  << AMMUNITION_STATE
                  << worm.worm_weapon_tepelport_.
          getCurrentAmmunition()
                  << ") *\n";
      }
      else
      {
        std::cout << "  " << worm.worm_weapon_tepelport_.getWeaponName()
                  << AMMUNITION_STATE
                  << worm.worm_weapon_tepelport_.
          getCurrentAmmunition()
                 <<")\n";
      }
    }
    else if(worm.worm_weapon_flamethrower_.getWeaponName() ==
     weapons[iterator] &&
            worm.worm_weapon_flamethrower_.getCurrentAmmunition() > 0)
    {
      if(worm.worm_weapon_flamethrower_.getWeaponName() ==
       worm.getCurrentWeapon())
      {
        std::cout <<"  "
                  << worm.worm_weapon_flamethrower_.getWeaponName()
                  << AMMUNITION_STATE
                  << worm.worm_weapon_flamethrower_.getCurrentAmmunition()
                  << ") *\n";
      }
      else
      {
        std::cout<< "  " << worm.worm_weapon_flamethrower_.getWeaponName()
                 << AMMUNITION_STATE
                 << worm.worm_weapon_flamethrower_.getCurrentAmmunition()
                 << ")\n";
      }
    }
    else if(worm.worm_weapon_melee_.getWeaponName() == weapons[iterator] &&
            worm.worm_weapon_melee_.getCurrentAmmunition() > 0)
    {
      if(worm.worm_weapon_melee_.getWeaponName() ==
       worm.getCurrentWeapon())
      {
        std::cout << "  " << worm.worm_weapon_melee_.getWeaponName()
                  << AMMUNITION_STATE
                  << worm.worm_weapon_melee_.getCurrentAmmunition()
                  << ") *\n";
      }
      else
      {
        std::cout << "  " << worm.worm_weapon_melee_.getWeaponName()
                  << AMMUNITION_STATE
                  << worm.worm_weapon_melee_.getCurrentAmmunition()
                  << ")\n";
      }
    }
    else if(worm.worm_weapon_airstrike_.getWeaponName() == weapons[iterator]
            && worm.worm_weapon_airstrike_.getCurrentAmmunition() > 0)
    {
      if(worm.worm_weapon_airstrike_.getWeaponName() ==
       worm.getCurrentWeapon())
      {
        std::cout << "  " << worm.worm_weapon_airstrike_.getWeaponName()
                  << AMMUNITION_STATE
                  << worm.worm_weapon_airstrike_.getCurrentAmmunition()
                  << ") *\n";
      }
      else
      {
        std::cout << "  "
                  << worm.worm_weapon_airstrike_.getWeaponName()
                  << AMMUNITION_STATE
                  << worm.worm_weapon_airstrike_.getCurrentAmmunition()
                  << ")\n";
      }
    }
  }
  std::cout << "\n";
  std::cout<<"worms of player 1\n";
  for(int iterator = 0; iterator < 
    static_cast <int> (worms_team_one.size()); iterator++)
  {
    if(worms_team_one[iterator].getWormId() > 0)
    {

      std::cout<<"  " << worms_team_one[iterator].getWormName()<<
               " (" << worms_team_one[iterator].getWormId()<<
               ") " << worms_team_one[iterator].getWormHp()<<
               " HP (" << worms_team_one[iterator].getWormPlaceRow()<<
               ", " << worms_team_one[iterator].getWormPlaceCol()<<") ~\n";
    }
  }

  std::cout<<"worms of player 2\n";
  for(int iterator = 0; iterator < 
    static_cast<int>(worms_team_two.size()); iterator++)
  {
    if(worms_team_two[iterator].getWormId() > 0)
    {
      std::cout<<"  " << worms_team_two[iterator].getWormName()<<
               " (" << worms_team_two[iterator].getWormId()<<
               ") " << worms_team_two[iterator].getWormHp()<<
               " HP (" << worms_team_two[iterator].getWormPlaceRow()<<
               ", " << worms_team_two[iterator].getWormPlaceCol()<<") *\n";
    }
  }
}


void Sep::Game::airstrike(int col)
{
  int row = 0;
  while(final_map_[row][col].getType() == field.AIR && row
                                                       < getBoardHeight() - 1)
    row++;

  if(final_map_[row][col].getType() == field.CHEST)
  {
    std::cout<< MESSAGE_SHOT_HIT_CHEST << row << ", " << col<<")\n";
    final_map_[row][col].setType_(field.AIR);
    final_map_[row][col].weapon_in_chest_ = NO_WEPON;
  }
  else if(final_map_[row][col].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << row << ", "<< col<<")\n";
    final_map_[row][col].setType_(field.AIR);
  }
  else if(final_map_[row][col]. pointer_to_worm_ != NULL)
  {
    if(final_map_[row][col]. pointer_to_worm_ -> getWormHp() >
      - AIRSTRIKE_DAMAGE)
    {
      final_map_[row][col]. pointer_to_worm_ -> setWormHp(AIRSTRIKE_DAMAGE);
      std::cout << MESSAGE_SHOT_HIT_WORM << row << ", " << col << ")\n";
      std::cout << final_map_[row][col]
        . pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[row][col].
         pointer_to_worm_->getWormId()
                << ") took 20hp damage" << std::endl;
    }
    else
    {
      std::cout << final_map_[row][col]. pointer_to_worm_->getWormName()
                << " (" << final_map_[0][0]. pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row][col]. pointer_to_worm_->setWormHp(0);
      final_map_[row][col]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row][col].setType_(field.AIR);
      final_map_[row][col].setWormPointerToNull();
    }
  }
  else if(final_map_[row][col].getType() == field.WATER)
  {
     
  }
  else if(final_map_[row][col].getType() == field.AIR)
  {
    std::cout << MESSAGE_SHOT_MISSED;
  }
}


int Sep::Game::teleport(int row, int col, Worm &worm_to_be_transported)
{
  int old_row = worm_to_be_transported.getWormPlaceRow();
  int old_col = worm_to_be_transported.getWormPlaceCol();
  int gravity_time = 0;
  

  if(!inField(row, col))
  {
    std::cout << ERROR_INVALID_TARGET << std::endl;
    return RETURN_CONTINUE;
  }

  if(final_map_[row + 1][col].getType() == field.WATER)
  {
    std::cout << worm_to_be_transported.getWormName()
                    << " (" << worm_to_be_transported.getWormId()
                    << ") drowned." << std::endl;
    worm_to_be_transported.setWormId(DEAD_ID);
    worm_to_be_transported.setWormHp(-worm_to_be_transported.getWormHp());
    final_map_[old_row][old_col]. pointer_to_worm_ = NULL;
    final_map_[old_row][old_col].setType_(field.AIR);
    return RETURN_OK;
  }



  if(final_map_[row][col].getType() == field.CHEST)
  {
    if(final_map_[row][col].weapon_in_chest_ == BAZOOKA)
    {
      worm_to_be_transported.worm_weapon_bazooka_.setCurrentAmmunition(1);
      std::cout << worm_to_be_transported.getWormName()
               << " (" << worm_to_be_transported.getWormId()
               << WORM_PICKED_UP_BAZOOKA_MESSAGE;
    }
    else if(final_map_[row][col].weapon_in_chest_ == TELEPORT)
    {
      worm_to_be_transported.worm_weapon_tepelport_.
        setCurrentAmmunition(1);
      std::cout << worm_to_be_transported.getWormName() << " ("
                << worm_to_be_transported.getWormId()
                << WORM_PICKED_UP_TELEPORT_MESSAGE;
    }
    else if(final_map_[row][col].weapon_in_chest_ == BLOWTORCH)
    {
      worm_to_be_transported.worm_weapon_flamethrower_.
        setCurrentAmmunition(1);
      std::cout << worm_to_be_transported.getWormName() << " ("
                << worm_to_be_transported.getWormId()
                << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
    }
    else if(final_map_[row][col].weapon_in_chest_ == AIRSTRIKE)
    {
      worm_to_be_transported.worm_weapon_airstrike_.
        setCurrentAmmunition(1);
      std::cout << worm_to_be_transported.getWormName()
                << " (" << worm_to_be_transported.getWormId()
                << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
    }
    else if(final_map_[row][col].weapon_in_chest_ == MELEE)
    {
      worm_to_be_transported.worm_weapon_melee_.setCurrentAmmunition(1);
      std::cout << worm_to_be_transported.getWormName()
                << " (" << worm_to_be_transported.getWormId()
                << WORM_PICKED_UP_MELEE_MESSAGE;
    }
    if(inField(row + 1, col))
    {
      if(final_map_[row + 1][col].getType() == field.CHEST)
      {
        if(final_map_[row + 1][col].weapon_in_chest_ == BAZOOKA)
        {
          worm_to_be_transported.worm_weapon_bazooka_.
            setCurrentAmmunition(1);
          std::cout << worm_to_be_transported.getWormName()
                    << " (" << worm_to_be_transported.getWormId()
                    << WORM_PICKED_UP_BAZOOKA_MESSAGE;
        }
        else if(final_map_[row + 1][col].weapon_in_chest_ == TELEPORT)
        {
          worm_to_be_transported.worm_weapon_tepelport_.
            setCurrentAmmunition(1);
          std::cout << worm_to_be_transported.getWormName()<<" ("
                    << worm_to_be_transported.getWormId()
                    << WORM_PICKED_UP_TELEPORT_MESSAGE;
        }
        else if(final_map_[row + 1][col].weapon_in_chest_ == BLOWTORCH)
        {
          worm_to_be_transported.worm_weapon_flamethrower_.
            setCurrentAmmunition(1);
          std::cout << worm_to_be_transported.getWormName()
                    << " (" << worm_to_be_transported.getWormId()
                    << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
        }
        else if(final_map_[row + 1][col].weapon_in_chest_ == AIRSTRIKE)
        {
          worm_to_be_transported.worm_weapon_airstrike_.
            setCurrentAmmunition(1);
          std::cout << worm_to_be_transported.getWormName()
                    << " (" << worm_to_be_transported.getWormId()
                    << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
        }
        else if(final_map_[row + 1][col].weapon_in_chest_ == MELEE)
        {
          worm_to_be_transported.worm_weapon_melee_.
            setCurrentAmmunition(1);
          std::cout << worm_to_be_transported.getWormName()
                    << " (" << worm_to_be_transported.getWormId()
                    << WORM_PICKED_UP_MELEE_MESSAGE;
        }
      }
    }

    if(worm_to_be_transported.getWormTeamNumber() == 1)
    {
      final_map_[row][col].setType_(field.WORM);
      final_map_[row][col]. pointer_to_worm_
        = final_map_[old_row][old_col]. pointer_to_worm_;
      final_map_[old_row][old_col]. pointer_to_worm_ = NULL;
      final_map_[old_row][old_col].setType_(field.AIR);
    }
    else if(worm_to_be_transported.getWormTeamNumber() == 2)
    {
      final_map_[row][col].setType_(field.WORM2);
      final_map_[row][col]. pointer_to_worm_ =
        final_map_[old_row][old_col]. pointer_to_worm_;
      final_map_[old_row][old_col]. pointer_to_worm_ = NULL;
      final_map_[old_row][old_col].setType_(field.AIR);
    }


    worm_to_be_transported.setWormPlaceRow(row);
    worm_to_be_transported.setWormPlaceCol(col);
    worm_to_be_transported.worm_weapon_tepelport_.
      setCurrentAmmunition(- 1);
    checkForChest(worm_to_be_transported);
  }
  else if(final_map_[row][col].getType() == field.AIR)
  {
    if(worm_to_be_transported.getWormTeamNumber() == 1)
    {
      final_map_[row][col].setType_(field.WORM);
      final_map_[row][col]. pointer_to_worm_ = final_map_[old_row]
      [old_col]. pointer_to_worm_;
      final_map_[old_row][old_col]. pointer_to_worm_ = NULL;
      final_map_[old_row][old_col].setType_(field.AIR);
    }
    else if(worm_to_be_transported.getWormTeamNumber() == 2)
    {
      final_map_[row][col].setType_(field.WORM2);
      final_map_[row][col]. pointer_to_worm_ = final_map_[old_row]
      [old_col]. pointer_to_worm_;
      final_map_[old_row][old_col]. pointer_to_worm_ = NULL;
      final_map_[old_row][old_col].setType_(field.AIR);
    }
    worm_to_be_transported.setWormPlaceRow(row);
    worm_to_be_transported.setWormPlaceCol(col);
    worm_to_be_transported.worm_weapon_tepelport_.
      setCurrentAmmunition(- 1);
    while (gravity_time < 20)
    {
      applyGravity(col);
      applyGravity(old_col);
      gravity_time++;
    }
    checkForChest(worm_to_be_transported);
    return RETURN_OK;
  }
  else
  {
    std::cout << ERROR_INVALID_TARGET << std::endl;
    return RETURN_CONTINUE;
  }
  return RETURN_OK;
}



int Sep::Game::moveOneStep(int direction, Worm &moving_worm)
{
  int width = getBoardWidth() - 1;
  int successful_move = 0;
  int worm_in_col = moving_worm.getWormPlaceCol();
  int worm_in_row = moving_worm.getWormPlaceRow();
  int gravity_time = 0;
  int return_value;
  int position;

  ///can move out of the map
  if(direction < 0 && (worm_in_col == 0))
  {
    std::cout << moving_worm.getWormName()<<" ("
              << moving_worm.getWormId()
             <<") fell out of the map.\n";
    final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
    final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
    moving_worm.setWormId(DEAD_ID);
    moving_worm.setWormHp(- moving_worm.getWormHp());
    return RETURN_INFO_WORM_DEAD;
  }
  if(direction > 0 && (worm_in_col == width))
  {
    std::cout << moving_worm.getWormName()<<" ("
              << moving_worm.getWormId()
             <<") fell out of the map.\n";
    final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
    final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
    moving_worm.setWormId(DEAD_ID);
    moving_worm.setWormHp(- moving_worm.getWormHp());
    return RETURN_INFO_WORM_DEAD;
  } // can move out of map

  if(final_map_[worm_in_row][worm_in_col + 1].getType() ==
     field.AIR && direction > 0)
  {
    if(final_map_[worm_in_row + 1][worm_in_col + 1].getType() ==
      field.AIR)
    {
      final_map_[worm_in_row][worm_in_col + 1].setType_
        (final_map_[worm_in_row][worm_in_col].getType());
      final_map_[worm_in_row][worm_in_col + 1]. pointer_to_worm_ =
        final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
      final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
      final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
      moving_worm.setWormPlaceCol(worm_in_col + 1);
      while(gravity_time < 20)
      {
        applyGravity((worm_in_col + 1));
        gravity_time++;
      }
      gravity_time = 0;
      while(gravity_time < 20)
      {
        applyGravity(worm_in_col);
        gravity_time++;
      }
      gravity_time = 0;
    }
    else
    {
      moving_worm.setWormPlaceCol(worm_in_col + 1);
      return_value = moveWorm(moving_worm, direction);
      if(return_value == - 11)
      {
        final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
        final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
        gravity_time = 0;
        while(gravity_time < 20)
        {
          applyGravity((worm_in_col + 1));
          gravity_time++;
        }
        gravity_time = 0;
        while(gravity_time < 20)
        {
          applyGravity(worm_in_col);
          gravity_time++;
        }
        return RETURN_ERROR_ARGUMENT;
      }
      else
      {
        final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
        final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
        gravity_time = 0;
        while(gravity_time < 20)
        {
          applyGravity((worm_in_col + 1));
          gravity_time++;
        }
        gravity_time = 0;
        while(gravity_time < 20)
        {
          applyGravity(worm_in_col);
          gravity_time++;
        }
        successful_move = 1;
      }
    }
  }

  if((final_map_[worm_in_row][worm_in_col - 1].getType() ==
    field.AIR) && direction < 0)
  {
    if(final_map_[worm_in_row + 1][worm_in_col - 1].getType() ==
      field.AIR)
    {
      final_map_[worm_in_row][worm_in_col - 1].setType_
        (final_map_[worm_in_row][worm_in_col].getType());
      final_map_[worm_in_row][worm_in_col - 1]. pointer_to_worm_ =
        final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
      final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
      final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
      moving_worm.setWormPlaceCol(worm_in_col - 1);
      while(gravity_time < 20)
      {
        applyGravity((worm_in_col - 1));
        gravity_time++;
      }
      gravity_time = 0;
      while(gravity_time < 20)
      {
        applyGravity(worm_in_col);
        gravity_time++;
      }
      gravity_time = 0;
    }
    else
    {
      moving_worm.setWormPlaceCol(worm_in_col - 1);
      return_value = moveWorm(moving_worm, direction);
      if(return_value == - 11)
      {
        final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
        final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
        gravity_time = 0;
        while(gravity_time < 20)
        {
          applyGravity((worm_in_col - 1));
          gravity_time++;
        }
        gravity_time = 0;
        while(gravity_time < 20)
        {
          applyGravity(worm_in_col);
          gravity_time++;
        }
        return RETURN_ERROR_ARGUMENT;
      }
      else
      {
        final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
        final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
        gravity_time = 0;
        while(gravity_time < 20)
        {
          applyGravity((worm_in_col - 1));
          gravity_time++;
        }
        gravity_time = 0;
        while(gravity_time < 20)
        {
          applyGravity(worm_in_col);
          gravity_time++;
        }
        successful_move = 1;
      }
    }
  }

  if(final_map_[worm_in_row][worm_in_col + 1].getType() ==
       field.CHEST && direction > 0)
  {
    moving_worm.setWormPlaceCol(worm_in_col + 1);
    return_value = moveWorm(moving_worm, direction);
    if(return_value == - 11)
    {
      final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
      final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
      return RETURN_ERROR_ARGUMENT;
    }
    else
    {
      final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
      final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
      if(final_map_[worm_in_row - 1][worm_in_col + 1].
        getType() == field.CHEST)
      {
        if(final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_ == BAZOOKA)
        {
          moving_worm.worm_weapon_bazooka_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_BAZOOKA_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_ == TELEPORT)
        {
          moving_worm.worm_weapon_tepelport_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName()
                    << " (" << moving_worm.getWormId()
                    << WORM_PICKED_UP_TELEPORT_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_ == BLOWTORCH)
        {
          moving_worm.worm_weapon_flamethrower_.setCurrentAmmunition(1);
          std::cout  << moving_worm.getWormName()
                    << " ("  << moving_worm.getWormId()
                    << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_ == AIRSTRIKE)
        {
          moving_worm.worm_weapon_airstrike_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_ == MELEE)
        {
          moving_worm.worm_weapon_melee_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName()<<" ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_MELEE_MESSAGE;
        }
        final_map_[worm_in_row - 1][worm_in_col + 1].setType_(field.AIR);
        final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_ = NO_WEPON;
      }
      successful_move = 1;
    }
  }

  if(final_map_[worm_in_row][worm_in_col - 1].getType()
     == field.CHEST && direction < 0)
  {
    moving_worm.setWormPlaceCol(worm_in_col - 1);
    return_value = moveWorm(moving_worm, direction);
    if(return_value == RETURN_INFO_WORM_DROWNED)
    {
      final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
      final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
      return RETURN_ERROR_ARGUMENT;
    }
    else
    {
      final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
      final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
      if(final_map_[worm_in_row - 1][worm_in_col - 1].getType()
         == field.CHEST)
      {
        if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_ == BAZOOKA)
        {
          moving_worm.worm_weapon_bazooka_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName()<<" ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_BAZOOKA_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_ == TELEPORT)
        {
          moving_worm.worm_weapon_tepelport_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName()<<" ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_TELEPORT_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_ == BLOWTORCH)
        {
          moving_worm.worm_weapon_flamethrower_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_
                == AIRSTRIKE)
        {
          moving_worm.worm_weapon_airstrike_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_
                == MELEE)
        {
          moving_worm.worm_weapon_melee_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName()<<" ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_MELEE_MESSAGE;
        }

        final_map_[worm_in_row - 1][worm_in_col - 1].setType_(field.AIR);
        final_map_[worm_in_row - 1][worm_in_col - 1].weapon_in_chest_
          = NO_WEPON;
      }
      successful_move = 1;
    }
  }


  ///When worm should be placed one step right
  if((direction > 0) && (final_map_[worm_in_row][worm_in_col + 1].
    getType() != field.AIR)
     && successful_move == 0)
  {
    if(inField(worm_in_row - 1, worm_in_col + 1))
    {
      if(final_map_[worm_in_row - 1][worm_in_col + 1].getType()
         == field.AIR)
      {
        if(moving_worm.getWormId() <= 3)
        {
          final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
          final_map_[worm_in_row - 1][worm_in_col + 1]. pointer_to_worm_ =
            final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
          final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
          final_map_[worm_in_row - 1][worm_in_col+ 1].setType_(field.WORM);
          moving_worm.setWormPlaceRow(worm_in_row - 1);
          moving_worm.setWormPlaceCol(worm_in_col + 1);

          successful_move = 1;
        }
        if(moving_worm.getWormId() > 3)
        {
          final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
          final_map_[worm_in_row - 1][worm_in_col + 1]. pointer_to_worm_ =
            final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
          final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
          final_map_[worm_in_row - 1][worm_in_col+ 1].setType_(field.WORM2);
          moving_worm.setWormPlaceRow(worm_in_row - 1);
          moving_worm.setWormPlaceCol(worm_in_col + 1);
          successful_move = 1;
        }

        checkForChest(moving_worm);
        while(gravity_time < 20)
        {
          applyGravity(moving_worm.getWormPlaceCol());
          gravity_time++;
        }
        gravity_time = 0;
      }
      else if(final_map_[worm_in_row - 1][worm_in_col + 1].getType()
              == field.CHEST)
      {


        if(final_map_[worm_in_row - 1][worm_in_col + 1].weapon_in_chest_
           == BAZOOKA)
        {
          moving_worm.worm_weapon_bazooka_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_BAZOOKA_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_
                == TELEPORT)
        {
          moving_worm.worm_weapon_tepelport_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_TELEPORT_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_
                == BLOWTORCH)
        {
          moving_worm.worm_weapon_flamethrower_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_
                == AIRSTRIKE)
        {
          moving_worm.worm_weapon_airstrike_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col + 1].
          weapon_in_chest_
                == MELEE)
        {
          moving_worm.worm_weapon_melee_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_MELEE_MESSAGE;
        }
        if(moving_worm.getWormId() <= 3)
        {
          final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
          final_map_[worm_in_row - 1][worm_in_col + 1]. pointer_to_worm_
            = final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
          final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
          final_map_[worm_in_row - 1][worm_in_col + 1].
            setType_(field.WORM);
          moving_worm.setWormPlaceRow(worm_in_row - 1);
          moving_worm.setWormPlaceCol(worm_in_col + 1);
          successful_move = 1;
        }
        if(moving_worm.getWormId() > 3)
        {
          final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
          final_map_[worm_in_row - 1][worm_in_col + 1]. pointer_to_worm_
            = final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
          final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
          final_map_[worm_in_row - 1][worm_in_col + 1].
            setType_(field.WORM2);
          moving_worm.setWormPlaceRow(worm_in_row - 1);
          moving_worm.setWormPlaceCol(worm_in_col + 1);
          successful_move = 1;
        }
        checkForChest(moving_worm);
        while(gravity_time < 20)
        {
          applyGravity(moving_worm.getWormPlaceCol());
          gravity_time++;
        }
        gravity_time = 0;
      }
      else
      {
        std::cout << MESSAGE_CANT_MOVE << std::endl;
        return RETURN_ERROR_ARGUMENT;
      }
    }
    else
    {
      std::cout << MESSAGE_CANT_MOVE << std::endl;
      return RETURN_ERROR_ARGUMENT;
    }
  }

  ///When worm should be placed one step left
  if((direction < 0) && (final_map_[worm_in_row]
                         [worm_in_col - 1].getType()
                         != field.AIR)
     && successful_move == 0)
  {
    if(inField(worm_in_row - 1, worm_in_col - 1))
    {
      if(final_map_[worm_in_row - 1][worm_in_col - 1].getType()
         == field.AIR)
      {
        if(moving_worm.getWormId() <= 3)
        {
          final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
          final_map_[worm_in_row - 1][worm_in_col - 1]. pointer_to_worm_
            = final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
          final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
          final_map_[worm_in_row - 1][worm_in_col- 1].setType_(field.WORM);
          moving_worm.setWormPlaceRow(worm_in_row - 1);
          moving_worm.setWormPlaceCol(worm_in_col - 1);
          successful_move = 1;
        }
        if(moving_worm.getWormId() > 3)
        {
          final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
          final_map_[worm_in_row - 1][worm_in_col - 1]. pointer_to_worm_
            = final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
          final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
          final_map_[worm_in_row - 1][worm_in_col- 1].
            setType_(field.WORM2);
          moving_worm.setWormPlaceRow(worm_in_row - 1);
          moving_worm.setWormPlaceCol(worm_in_col - 1);
          successful_move = 1;
        }
        checkForChest(moving_worm);
        while(gravity_time < 20)
        {
          applyGravity(moving_worm.getWormPlaceCol());
          gravity_time++;
        }
        gravity_time = 0;
      }
      else if(final_map_[worm_in_row - 1][worm_in_col - 1].getType()
              == field.CHEST)
      {
        if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_ == BAZOOKA)
        {
          moving_worm.worm_weapon_bazooka_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_BAZOOKA_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_ == TELEPORT)
        {
          moving_worm.worm_weapon_tepelport_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_TELEPORT_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_
                == BLOWTORCH)
        {
          moving_worm.worm_weapon_flamethrower_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_
                == AIRSTRIKE)
        {
          moving_worm.worm_weapon_airstrike_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() <<" ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
        }
        else if(final_map_[worm_in_row - 1][worm_in_col - 1].
          weapon_in_chest_
                == MELEE)
        {
          moving_worm.worm_weapon_melee_.setCurrentAmmunition(1);
          std::cout << moving_worm.getWormName() << " ("
                    << moving_worm.getWormId()
                    << WORM_PICKED_UP_MELEE_MESSAGE;
        }
        if(moving_worm.getWormId() <= 3)
        {
          final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
          final_map_[worm_in_row - 1][worm_in_col - 1]. pointer_to_worm_
            = final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
          final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
          final_map_[worm_in_row - 1][worm_in_col- 1].setType_(field.WORM);
          moving_worm.setWormPlaceRow(worm_in_row - 1);
          moving_worm.setWormPlaceCol(worm_in_col - 1);
          successful_move = 1;
        }
        if(moving_worm.getWormId() > 3)
        {
          final_map_[worm_in_row][worm_in_col].setType_(field.AIR);
          final_map_[worm_in_row - 1][worm_in_col - 1]. pointer_to_worm_
            = final_map_[worm_in_row][worm_in_col]. pointer_to_worm_;
          final_map_[worm_in_row][worm_in_col].setWormPointerToNull();
          final_map_[worm_in_row - 1][worm_in_col- 1].setType_(field.WORM2);
          moving_worm.setWormPlaceRow(worm_in_row - 1);
          moving_worm.setWormPlaceCol(worm_in_col - 1);
          successful_move = 1;
        }
        checkForChest(moving_worm);
        while(gravity_time < 20)
        {
          applyGravity(moving_worm.getWormPlaceCol());
          gravity_time++;
        }
        gravity_time = 0;
      }
      else
      {
        std::cout << MESSAGE_CANT_MOVE << std::endl;
        return RETURN_ERROR_ARGUMENT;
      }
    }
    else
    {
      std::cout << MESSAGE_CANT_MOVE << std::endl;
      return RETURN_ERROR_ARGUMENT;
    }
  }

  if(inField(worm_in_row - 1, worm_in_col))
  {
    if (successful_move == 1 &&
      final_map_[worm_in_row - 1][worm_in_col].getType()
      == field.WORM)
    {
      position = worm_in_row - 1;
      while (final_map_[position][worm_in_col].getType()
             == field.WORM && position >= 0)
      {
        position--;
      }
      final_map_[position + 1][worm_in_col].setType_(field.AIR);
      final_map_[worm_in_row][worm_in_col].setType_(field.WORM);
      final_map_[worm_in_row][worm_in_col]. pointer_to_worm_ =
        final_map_[position + 1][worm_in_col]. pointer_to_worm_;
      final_map_[position + 1][worm_in_col].setWormPointerToNull();
      final_map_[worm_in_row][worm_in_col]. pointer_to_worm_ ->
        setWormPlaceRow(worm_in_row);
      final_map_[worm_in_row][worm_in_col]. pointer_to_worm_ ->
        setWormPlaceCol(worm_in_col);
    }

    if (successful_move == 1 &&
      final_map_[worm_in_row - 1][worm_in_col].getType()
      == field.WORM2)
    {
      position = worm_in_row - 1;
      while (final_map_[position][worm_in_col].getType()
             == field.WORM2 && position >= 0)
      {
        position--;
      }
      final_map_[position + 1][worm_in_col].setType_(field.AIR);
      final_map_[worm_in_row][worm_in_col].setType_(field.WORM2);
      final_map_[worm_in_row][worm_in_col]. pointer_to_worm_
        = final_map_[position + 1][worm_in_col]. pointer_to_worm_;
      final_map_[position + 1][worm_in_col].setWormPointerToNull();
      final_map_[worm_in_row][worm_in_col]. pointer_to_worm_ ->
        setWormPlaceRow(worm_in_row);
      final_map_[worm_in_row][worm_in_col]. pointer_to_worm_ ->
        setWormPlaceCol(worm_in_col);

    }
  }

  return RETURN_OK;

}

void Sep::Game::checkForChest(Worm &worm_around_chest)
{
  int row = worm_around_chest.getWormPlaceRow();
  int col = worm_around_chest.getWormPlaceCol();

  while(row > 0)
  {
    if(final_map_[row - 1][col].getType() == field.CHEST)
    {
      if(final_map_[row - 1][col].weapon_in_chest_ == BAZOOKA)
      {
        worm_around_chest.worm_weapon_bazooka_.setCurrentAmmunition(1);
        std::cout<<worm_around_chest.getWormName()
               <<" ("<<worm_around_chest.getWormId()
               <<") picked up 1 of bazooka\n";
      }
      else if(final_map_[row - 1][col].weapon_in_chest_
            == TELEPORT)
      {
        worm_around_chest.worm_weapon_tepelport_.setCurrentAmmunition(1);
        std::cout<<worm_around_chest.getWormName()<<" ("
               <<worm_around_chest.getWormId()
               <<") picked up 1 of teleport\n";
      }
      else if(final_map_[row - 1][col].weapon_in_chest_
            == BLOWTORCH)
      {
        worm_around_chest.worm_weapon_flamethrower_.setCurrentAmmunition(1);
        std::cout<<worm_around_chest.getWormName()<<" ("
               <<worm_around_chest.getWormId()
               <<") picked up 1 of blowtorch\n";
      }
      else if(final_map_[row - 1][col].weapon_in_chest_
            == AIRSTRIKE)
      {
        worm_around_chest.worm_weapon_airstrike_.setCurrentAmmunition(1);
        std::cout<<worm_around_chest.getWormName()<<" ("
               <<worm_around_chest.getWormId()
               <<") picked up 1 of airstrike\n";
      }
      else if(final_map_[row - 1][col].weapon_in_chest_
            == MELEE)
      {
        worm_around_chest.worm_weapon_melee_.setCurrentAmmunition(1);
        std::cout<<worm_around_chest.getWormName()<<" ("
               <<worm_around_chest.getWormId()
               <<") picked up 1 of melee\n";
      }
      final_map_[row - 1][col].setType_(field.AIR);
      final_map_[row - 1][col].weapon_in_chest_ = NO_WEPON;
    }
    row--;
  }
}


void Sep::Game::printHelp()
{
  std::cout << "Available Commands:\n"
    "  move [left/right] [0..3]\n"
    "    Move your worm.\n"
    "  choose [weapon]\n"
    "    Select your weapon.\n"
    "  action [l/r/d/u/ld/rd/lu/ru]\n"
    "    Shoots current weapon.\n"
    "    Activates blowtorch.\n"
    "  action\n"
    "    Activates melee attack.\n"
    "  action [row] [col]\n"
    "    Teleports to coordinate.\n"
    "  action [col]\n"
    "    Airstrike.\n"
    "  action idle\n"
    "    Do nothing.\n"
    "  quit\n"
    "    End the game.\n"
    "  help\n"
    "    Display this help.\n"
    "  state\n"
    "    Print the current game state.\n";
}

void Sep::Game::move(bool &was_move, Worm &moving_worm,
                     std::string direction,
                     int steps)
{
  int width = getBoardWidth();
  int height = getBoardHeight();

  int row = moving_worm.getWormPlaceRow();
  int col = moving_worm.getWormPlaceCol();
  int return_value;

  if(direction.find("L") != std::string::npos)
  {
    steps = -steps;
  }
  else if(direction.find("R") != std::string::npos)
  {
    steps = steps;
  }
  else
  {
    std::cout << ERROR_INVALID_PARAMETER;
  }

  if(row > height)
  {
    std::cout << ERROR_INVALID_TARGET << std::endl;
    return;
  }
  if(col > width)
  {
    std::cout << ERROR_INVALID_TARGET << std::endl;
    return;
  }

  Field temp_var_want = final_map_[row][col];

  if(steps < - 3)
    steps = - 3;
  if(steps > 3)
    steps = 3;
  return_value = 0;

  if(temp_var_want.getType() != field.WORM &&
    temp_var_want.getType() != field.WORM2)
  {
    std::cout << ERROR_INVALID_TARGET << std::endl;
    return;
  }
  was_move = true;
  if(steps == 0)
    return;

  if(steps == 1 || steps == - 1)
  {
    moveOneStep(steps, moving_worm);
    return;
  }
  if(steps == 2)
  {
    return_value = moveOneStep(1, moving_worm);
    if(return_value == 0 && moving_worm.getWormId() != DEAD_ID)
    {
      moveOneStep(1, moving_worm);
    }
    return;
  }

  if(steps == - 2)
  {
    return_value = moveOneStep(- 1, moving_worm);
    if(return_value == 0 && moving_worm.getWormId() != DEAD_ID)
    {
      moveOneStep(- 1, moving_worm);
    }
    return;
  }

  if(steps == 3)
  {
    return_value = moveOneStep(1, moving_worm);
    if(return_value == 0 && moving_worm.getWormId() != DEAD_ID)
    {
      return_value = moveOneStep(1, moving_worm);
      if(return_value == 0 && moving_worm.getWormId() != DEAD_ID)
      {
        moveOneStep(1, moving_worm);
      }
      else
        return;
    }
    else
      return;
  }

  if(steps == - 3)
  {
    return_value = moveOneStep(- 1, moving_worm);
    if(return_value == 0 && moving_worm.getWormId() != DEAD_ID)
    {
      return_value = moveOneStep(- 1, moving_worm);
      if(return_value == 0 && moving_worm.getWormId() != DEAD_ID)
      {
        moveOneStep(- 1, moving_worm);
      }
      else
        return;
    }
    else
      return;
  }
}

int Sep::Game::actionDecider(std::string action, Worm &worm_in_game,
                              std::vector<Sep::Worm> &team_one_worms,
                             std::vector<Sep::Worm> &team_two_worms,
                             bool &was_move_command)
{
  int number_of_worms_1;
  int number_of_worm_2;
  std::transform(action.begin(), action.end(), action.begin(), ::toupper);
  int return_value = 0;
  std::istringstream ss(action);
  std::vector<std::string> input;
  int steps;
  int row;
  int col;

  while(!(ss.eof()))
  {
    std::string temp_space = " ";
    ss>>temp_space;
    if(temp_space != " ")
    {
      input.push_back(temp_space);
    }
  }

  if(input.empty())
    return RETURN_CONTINUE;

  if(std::string::npos != input[0].find("STATE") 
                         && input[0].size() == 5 
                            && input.size() > 1)
  {
    std::cout << ERROR_WRONG_PARAMETAR_COUNT << std::endl;
    return RETURN_CONTINUE;
  }

  if(std::string::npos != input[0].find("ACTION") 
                          && input[0].size() == 6
                             && input.size() > 3)
  {
    std::cout << ERROR_WRONG_PARAMETAR_COUNT << std::endl;
    return RETURN_CONTINUE;
  }

  if(std::string::npos != input[0].find("HELP") 
                        && input[0].size() == 4
                           && input.size() > 1)
  {
    std::cout << ERROR_WRONG_PARAMETAR_COUNT << std::endl;
    return RETURN_CONTINUE;
  }

  if(std::string::npos != input[0].find("MAP")
                       && input[0].size() == 3
                          && input.size() > 1)
  {
    std::cout << ERROR_WRONG_PARAMETAR_COUNT << std::endl;
    return RETURN_CONTINUE;
  }

  if(std::string::npos != input[0].find("QUIT")
                        && input[0].size() == 4
                           && input.size() > 1)
  {
    std::cout << ERROR_WRONG_PARAMETAR_COUNT << std::endl;
    return RETURN_CONTINUE;
  }

  if(std::string::npos != input[0].find("CHOOSE")
                          && input[0].size() == 6
                             && input.size() > 2)
  {
    std::cout << ERROR_WRONG_PARAMETAR_COUNT << std::endl;
    return RETURN_CONTINUE;
  }

  if(input.size() == 3)
  {
    if(std::string::npos != input[0].find("MOVE")
       && input[0].size() == 4 )
    {
      if(is_number(input[2]) && !was_move_command)
      {
        steps = std::stoi(input[2]);
        std::string direction;
        std::istringstream is(input[1]);
        is >> direction;
        if(steps < 0 || steps > 3)
        {
          std::cout<< ERROR_INVALID_PARAMETER;
          return RETURN_CONTINUE;
        }
        if(direction != "L" && direction != "R")
        {
          std::cout<< ERROR_INVALID_PARAMETER;
          return RETURN_CONTINUE;
        }
        move(was_move_command, worm_in_game, direction, steps);
        printFinalMap();
        if(worm_in_game.getWormId() < 0)
          addChest();
        return_value = checkForCasualities(team_one_worms,
                                           team_two_worms,
                                           worm_in_game,
                                           number_of_worms_1,
                                           number_of_worm_2);
        if(return_value == RETURN_INFO_END_DRAW ||
          return_value == RETURN_INFO_END_WINNER)
          return return_value;
        else if(return_value == RETURN_INFO_WORM_DEAD)
        {
          return return_value;
        }
        else
        {
          return RETURN_CONTINUE;
        }

      }
      else if(was_move_command)
      {
        std::cout << ERROR_COMMAND_NOT_ALLOWED
                  << std::endl;
        return RETURN_CONTINUE;
      }
      else
      {
        std::cout << ERROR_INVALID_PARAMETER;
        return RETURN_CONTINUE;
      }
    }
    else if(std::string::npos != input[0].find("ACTION")
            && input[0].size() == 6)
    {
      if(worm_in_game.getCurrentWeapon() == worm_in_game.
        worm_weapon_tepelport_.
        getWeaponName())
      {
        if(is_number(input[1]) && is_number(input[2]))
        {
          row = std::stoi(input[1]);
          col = std::stoi(input[2]);

          return_value = teleport(row, col, worm_in_game);
          if(return_value == RETURN_CONTINUE)
            return return_value;
          addChest();
          return_value = checkForCasualities(team_one_worms,
                                             team_two_worms,
                                             worm_in_game,
                                             number_of_worms_1,
                                             number_of_worm_2);
          return return_value;
        }
        else
        {
          std::cout << ERROR_INVALID_PARAMETER;
          return RETURN_CONTINUE;
        }
      }
      else
      {
        std::cout << ERROR_COMMAND_NOT_ALLOWED
                  << std::endl;
        return RETURN_CONTINUE;
      }

    }
    else
    {
      std::cout<< ERROR_UNKNOWN_COMMAND << std::endl;
      return RETURN_CONTINUE;
    }
  }
  else if(input.size() == 1)
  {
    if(std::string::npos != input[0].find("MOVE")
       && input[0].size() == 4 )
    {
      std::cout << ERROR_WRONG_PARAMETAR_COUNT << std::endl;
      return RETURN_CONTINUE;
    }
    if(std::string::npos != input[0].find("STATE")
       && input[0].size() == 5)
    {
      state(worm_in_game, team_one_worms, team_two_worms);
      return RETURN_CONTINUE;
    }
    else if(std::string::npos != input[0].find("MAP")
            && input[0].size() == 3)
    {
      printFinalMap();
      return RETURN_CONTINUE;
    }
    else if(std::string::npos != input[0].find("ACTION")
            && input[0].size() == 6)
    {
      if(worm_in_game.getCurrentWeapon() != worm_in_game.
        worm_weapon_melee_.
        getWeaponName())
      {
        std::cout << ERROR_COMMAND_NOT_ALLOWED
                  << std::endl;
        return RETURN_CONTINUE;
      }
      if(worm_in_game.worm_weapon_melee_.
        getCurrentAmmunition() < 1)
      {
        std::cout << ERROR_NO_AMMUNITION;
        return RETURN_CONTINUE;
      }

      actionMelee(worm_in_game);
      worm_in_game.worm_weapon_melee_.setCurrentAmmunition(- 1);
      addChest();
      return_value = checkForCasualities(team_one_worms,
                                         team_two_worms,
                                         worm_in_game,
                                         number_of_worms_1,
                                         number_of_worm_2);
      return return_value;
    }///Is wanted command "help"?
    else if(std::string::npos != input[0].find("HELP")
            && input[0].size() == 4)
    {
      printHelp();
      return RETURN_CONTINUE;
    }///Is wanted command "quit"?
    else if(std::string::npos != input[0].find("QUIT")
            && input[0].size() == 4)
    {
      return RETURN_QUIT;
    }
    else
    {
      std::cout << ERROR_UNKNOWN_COMMAND << std::endl;
      return RETURN_CONTINUE;
    }
  }/// Two words on input
  else if(input.size() == 2)
  { ///User wants to change current weapon of given worm?
    if(std::string::npos != input[0].find("MOVE")
       && input[0].size() == 4 )
    {
      std::cout << ERROR_WRONG_PARAMETAR_COUNT << std::endl;
      return RETURN_CONTINUE;
    }

    if(std::string::npos != input[0].find("CHOOSE") && input[0].
      size() == 6)
    {
      action.erase(action.begin(), action.end());

      std::transform(input[1].begin(), input[1].end()
        , input[1].begin(), ::tolower);
      choose(input[1], worm_in_game);
      return RETURN_CONTINUE;
    }
    ///First word Action?
    if(std::string::npos != input[0].find("ACTION") && input[0].
      size() == 6)
    {
      if(std::string::npos != input[1].find("IDLE") && input[1].
        size() == 4)
      {
        addChest();
        return RETURN_OK;
      }
      else if(worm_in_game.getCurrentWeapon()
              == worm_in_game.worm_weapon_tepelport_.getWeaponName())
      {
        std::cout << ERROR_COMMAND_NOT_ALLOWED
                  << std::endl;
        return RETURN_CONTINUE;
      }
      else if(is_number(input[1]))
      {
        col = std::stoi(input[1]);
        if(col < getBoardWidth())
        {
          if(worm_in_game.getCurrentWeapon() != worm_in_game.
            worm_weapon_airstrike_.
            getWeaponName())
          {
            std::cout << ERROR_COMMAND_NOT_ALLOWED
                      << std::endl;
            return RETURN_CONTINUE;
          }
          if(worm_in_game.worm_weapon_airstrike_.
            getCurrentAmmunition() < 1)
          {
            std::cout << ERROR_NO_AMMUNITION;
            return RETURN_CONTINUE;
          }

          airstrike(col);
          worm_in_game.worm_weapon_airstrike_.setCurrentAmmunition(- 1);
          addChest();
          return_value = checkForCasualities(team_one_worms,
                                             team_two_worms,
                                             worm_in_game,
                                             number_of_worms_1,
                                             number_of_worm_2);
          return return_value;
        }
        else
        {
          std::cout<< ERROR_INVALID_PARAMETER;
          return RETURN_CONTINUE;
        }
      }
      else if(input[1] == "L" ||
               input[1] == "R" ||
               input[1] == "U" ||
               input[1] == "D" ||
               input[1] == "LU" ||
               input[1] == "LD" ||
               input[1] == "RU" ||
               input[1] == "RD")
      {
        ///Shoot with current weapon in given direction
        if(worm_in_game.getCurrentWeapon() ==
         worm_in_game.worm_weapon_bazooka_.
          getWeaponName())
        {
          worm_in_game.worm_weapon_bazooka_.setCurrentAmmunition(- 1);
          std::transform(input[1].begin(), input[1].end()
            , input[1].begin(), ::tolower);
          shootBazooka(worm_in_game, input[1]);
          addChest();
          return_value = checkForCasualities(team_one_worms,
                                             team_two_worms,
                                             worm_in_game,
                                             number_of_worms_1,
                                             number_of_worm_2);
          return return_value;
        }
        else if(worm_in_game.getCurrentWeapon() ==
          worm_in_game.worm_weapon_flamethrower_.getWeaponName())
        {
          worm_in_game.worm_weapon_flamethrower_.setCurrentAmmunition(- 1);
          std::transform(input[1].begin(), input[1].end()
            , input[1].begin(), ::tolower);
          shootBlowtorch(worm_in_game, input[1]);
          addChest();
          return_value = checkForCasualities(team_one_worms,
                                             team_two_worms,
                                             worm_in_game,
                                             number_of_worms_1,
                                             number_of_worm_2);
          return return_value;
        }
        else if(worm_in_game.getCurrentWeapon() == worm_in_game.
          worm_weapon_gun_.getWeaponName())
        {
          std::transform(input[1].begin(), input[1].end(),
                         input[1].begin(), ::tolower);
          shootGun(worm_in_game, input[1]);
          addChest();
          return_value = checkForCasualities(team_one_worms,
                                             team_two_worms,
                                             worm_in_game,
                                             number_of_worms_1,
                                             number_of_worm_2);
          return return_value;
        }
      }
      else
      {
        std::cout<< ERROR_INVALID_PARAMETER;
        return RETURN_CONTINUE;
      }
    }
    else
    {
      std::cout<< ERROR_UNKNOWN_COMMAND << std::endl;
      return RETURN_CONTINUE;
    }
  }
  else
  {
    std::cout<< ERROR_UNKNOWN_COMMAND << std::endl;
    return RETURN_CONTINUE;
  }

  return return_value;
}


bool Sep::Game::is_number(const std::string& s)
{
  int digit_counter = 0;
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it))
  { 
   ++it;
   ++digit_counter;
  }
  if(digit_counter > 9)
    return false;
  return !s.empty() && it == s.end();
}

void Sep::Game::makeMapOfFields()
{
  int row_iterator = 0;
  int column_iterator = - 1;
  Sep::Field temp_object;

  std::vector<std::vector<Sep::Field::FieldType>>::const_iterator row;
  std::vector<Sep::Field::FieldType>::const_iterator col;
  for(row = map_.begin(); row != map_.end(); ++row)
  {
    for(col = row->begin(); col != row->end(); ++col)
    {
      column_iterator++;
      final_map_[row_iterator].push_back(temp_object);
      final_map_[row_iterator][column_iterator].setType_(*col);
    }
    column_iterator = - 1;
    row_iterator++;
  }
}


void Sep::Game::printFinalMap()
{
  int width = getBoardWidth();
  int height = getBoardHeight();
  int row_iterator = 0;
  int column_iterator = 0;

  std::string first_and_last_line(static_cast<unsigned long>(width), 'A');
  for(row_iterator = 0; row_iterator < (width); row_iterator++)
  {
    if(row_iterator == width) break;
    else
    {
      if(column_iterator % 10 == 0)
        column_iterator = 0;
      first_and_last_line[row_iterator] += (char)column_iterator - 17;
    }
    column_iterator++;
  }
  row_iterator = 0;
  std::cout << "Current Map:" << std::endl;
  std::cout << std::setw(width + 2);
  std::cout << first_and_last_line << std::endl;
  std::vector< std::vector<Field>>::iterator row;
  std::vector<Field>::iterator col;
  for (row = final_map_.begin(); row != (final_map_.begin() + height); row++)
  {
    std::cout << std::setw(2);
    std::cout << row_iterator;
    for(col = row->begin(); col != row->end(); col++)
    {
      std::cout << field.getCharacter(col->getType());
    }
    std::cout << std::setw(2);
    std::cout << row_iterator << std::endl;
    row_iterator++;
  }
  std::cout << std::setw(width + 2);
  std::cout << first_and_last_line << std::endl;
};

void Sep::Game::addPointersOnWorms(std::vector<Sep::Worm> &worms_of_player)
{
  for(int iterator = 0; iterator <
   static_cast <int> (worms_of_player.size()); iterator++)
  {
    int row = worms_of_player[iterator].getWormPlaceRow();
    int col = worms_of_player[iterator].getWormPlaceCol();
    final_map_[row][col]. pointer_to_worm_ = &worms_of_player[iterator];
  }
}

void Sep::Game::addChest()
{
  int chest_in_col = random.getRandomInt(0, (getBoardWidth() - 1));
  int wepon_number = random.getRandomInt(0, (NUMBER_OF_WEAPONS - 1));
  int chest_in_row = 0;


  /// Column is full, no place for chest?
  if(final_map_[chest_in_row][chest_in_col].getType() != field.AIR)
    return;

  while((final_map_[chest_in_row][chest_in_col].getType() == field.AIR)
        && (chest_in_row < (getBoardHeight() - 1)))
  {
    chest_in_row++;
  }

  if(final_map_[chest_in_row][chest_in_col].getType() == field.WATER)
    return;
  if(final_map_[chest_in_row][chest_in_col].getType() == field.AIR)
    return;
  else
  {
    final_map_[chest_in_row - 1][chest_in_col].setType_(field.CHEST);
    final_map_[chest_in_row - 1][chest_in_col].weapon_in_chest_ =
      wepon_number;
    /// Worm is directly underneath?
    if(final_map_[chest_in_row][chest_in_col]. pointer_to_worm_ != NULL)
    {
      switch (wepon_number)
      {
        case BAZOOKA:
          final_map_[chest_in_row][chest_in_col]. pointer_to_worm_
            ->worm_weapon_bazooka_.setCurrentAmmunition(1);
          std::cout << final_map_[chest_in_row][chest_in_col].
               pointer_to_worm_->getWormName()
            << " ("
            << final_map_[chest_in_row][chest_in_col]. pointer_to_worm_
            ->getWormId()
            << WORM_PICKED_UP_BAZOOKA_MESSAGE;
          break;
        case TELEPORT:
          final_map_[chest_in_row][chest_in_col]. pointer_to_worm_
            ->worm_weapon_tepelport_.setCurrentAmmunition(1);
          std::cout << final_map_[chest_in_row][chest_in_col].
               pointer_to_worm_
            ->getWormName()
            << " ("
            << final_map_[chest_in_row][chest_in_col]. pointer_to_worm_
            ->getWormId()
            << WORM_PICKED_UP_TELEPORT_MESSAGE;
          break;
        case BLOWTORCH:
          final_map_[chest_in_row][chest_in_col]. pointer_to_worm_
            ->worm_weapon_flamethrower_.setCurrentAmmunition(1);
          std::cout << final_map_[chest_in_row][chest_in_col].
               pointer_to_worm_->getWormName()
            << " ("
            << final_map_[chest_in_row][chest_in_col]. pointer_to_worm_
            ->getWormId()
            << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
          break;
        case MELEE:
          final_map_[chest_in_row][chest_in_col]. pointer_to_worm_
            ->worm_weapon_melee_.setCurrentAmmunition(1);
          std::cout << final_map_[chest_in_row][chest_in_col].
               pointer_to_worm_->getWormName()
            << " ("
            << final_map_[chest_in_row][chest_in_col]. pointer_to_worm_
            ->getWormId()
            << WORM_PICKED_UP_MELEE_MESSAGE;
          break;
        case AIRSTRIKE:
          final_map_[chest_in_row][chest_in_col]. pointer_to_worm_
            ->worm_weapon_airstrike_.setCurrentAmmunition(1);
          std::cout << final_map_[chest_in_row][chest_in_col].
               pointer_to_worm_->getWormName()
            << " ("
            << final_map_[chest_in_row][chest_in_col].
                 pointer_to_worm_->getWormId()
            << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
          break;
      }
      final_map_[chest_in_row - 1][chest_in_col].setType_(field.AIR);
      final_map_[chest_in_row - 1][chest_in_col].
        weapon_in_chest_ = NO_WEPON;
    }
  }
  return;
}


void Sep::Game::applyGravity(int column)
{

  int row = 0;
  int wepon_number;
  int iterator;
  int field_iterator;
  while((row + 1) < getBoardHeight())
  {
    /// When chest hangs in the air.
    if(final_map_[row + 1][column].getType() == field.AIR
       && final_map_[row][column].getType() == field.CHEST)
    {
      iterator = row;
      wepon_number = final_map_[row][column].weapon_in_chest_;
      final_map_[row][column].setType_(field.AIR);
      final_map_[row][column].weapon_in_chest_ = NO_WEPON;

      while((final_map_[iterator][column].getType() == field.AIR)
            && (iterator < (getBoardHeight() - 1)))
      {

        iterator++;
      }
      if(final_map_[iterator][column].getType() == field.WATER)
        return;

      if((final_map_[iterator][column].getType() == field.AIR))
        return;
      else
      {
        final_map_[iterator - 1][column].setType_(field.CHEST);
        final_map_[iterator - 1][column].weapon_in_chest_
          = wepon_number;
        /// Worm is directly underneath?
        if(final_map_[iterator][column]. pointer_to_worm_ != NULL)
        {
          switch (wepon_number)
          {
            case BAZOOKA:
              final_map_[iterator][column]. pointer_to_worm_
                ->worm_weapon_bazooka_.setCurrentAmmunition(1);
              std::cout << final_map_[iterator][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[iterator][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_BAZOOKA_MESSAGE;
              break;
            case TELEPORT:
              final_map_[iterator][column]. pointer_to_worm_
                ->worm_weapon_tepelport_.setCurrentAmmunition(1);
              std::cout << final_map_[iterator][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[iterator][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_TELEPORT_MESSAGE;
              break;
            case BLOWTORCH:
              final_map_[iterator][column]. pointer_to_worm_
                ->worm_weapon_flamethrower_.setCurrentAmmunition(1);
              std::cout << final_map_[iterator][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[iterator][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
              break;
            case MELEE:
              final_map_[iterator][column]. pointer_to_worm_
                ->worm_weapon_melee_.setCurrentAmmunition(1);
              std::cout << final_map_[iterator][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[iterator][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_MELEE_MESSAGE;
              break;
            case AIRSTRIKE:
              final_map_[iterator][column]. pointer_to_worm_
                ->worm_weapon_airstrike_.setCurrentAmmunition(1);
              std::cout << final_map_[iterator][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[iterator][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
              break;
          }
          final_map_[iterator - 1][column].setType_(field.AIR);
          final_map_[iterator - 1][column].weapon_in_chest_ = NO_WEPON;
        }
      }
      return;
    }
    ///Case when Worm hangs in air:
    else if(final_map_[row + 1][column].getType() == field.AIR
            && final_map_[row][column]. pointer_to_worm_ != NULL)
    {
      field_iterator = row;
      field_iterator = row + 1;

      while(final_map_[field_iterator][column].getType() == field.AIR
            && field_iterator < (getBoardHeight() - 1))
      {
        field_iterator++;
        if(final_map_[field_iterator][column].getType() == field.CHEST)
        {
          wepon_number = final_map_[field_iterator][column].
            weapon_in_chest_;
          final_map_[field_iterator][column].setType_(field.AIR);
          final_map_[field_iterator][column].weapon_in_chest_ = NO_WEPON;
          switch (wepon_number)
          {
            case BAZOOKA:
              final_map_[row][column]. pointer_to_worm_
                ->worm_weapon_bazooka_.setCurrentAmmunition(1);
              std::cout << final_map_[row][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[row][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_BAZOOKA_MESSAGE;
              break;
            case TELEPORT:
              final_map_[row][column]. pointer_to_worm_
                ->worm_weapon_tepelport_.setCurrentAmmunition(1);
              std::cout << final_map_[row][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[row][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_TELEPORT_MESSAGE;
              break;
            case BLOWTORCH:
              final_map_[row][column]. pointer_to_worm_
                ->worm_weapon_flamethrower_.setCurrentAmmunition(1);
              std::cout << final_map_[row][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[row][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
              break;
            case MELEE:
              final_map_[row][column]. pointer_to_worm_
                ->worm_weapon_melee_.setCurrentAmmunition(1);
              std::cout << final_map_[row][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[row][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_MELEE_MESSAGE;
              break;
            case AIRSTRIKE:
              final_map_[row][column]. pointer_to_worm_
                ->worm_weapon_airstrike_.setCurrentAmmunition(1);
              std::cout << final_map_[row][column]. pointer_to_worm_
                ->getWormName()
                        << " ("
                        << final_map_[row][column]. pointer_to_worm_
                          ->getWormId()
                        << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
              break;
          }
        }
      }
      /// Worm at (0, 0) falls out of the map.
      if((field_iterator == (getBoardHeight() - 1) )
         && (final_map_[field_iterator][column].getType() == field.AIR))
      {
        std::cout << final_map_[row][column]. pointer_to_worm_
          ->getWormName()
                  << " (" << final_map_[row][column].
           pointer_to_worm_->getWormId()
                  << ") fell out of the map." << std::endl;
        final_map_[row][column]. pointer_to_worm_->setWormHp(0);
        final_map_[row][column]. pointer_to_worm_->setWormId(DEAD_ID);
        final_map_[row][column].setWormPointerToNull();
        final_map_[row][column].setType_(field.AIR);
        return;
      }
      else if((field_iterator - 1) == (row + 1))
      {
        if(final_map_[field_iterator][column].getType() == field.WATER)
        {
          std::cout << final_map_[row][column]. pointer_to_worm_
            ->getWormName()
                    << " (" << final_map_[row][column].
             pointer_to_worm_->getWormId()
                    << DROWNED_MESSAGE;
          final_map_[row][column]. pointer_to_worm_->setWormHp(0);
          final_map_[row][column]. pointer_to_worm_->setWormId(DEAD_ID);
          final_map_[row][column].setWormPointerToNull();
          final_map_[row][column].setType_(field.AIR);
          return;
        }

        final_map_[field_iterator - 1][column]. pointer_to_worm_ =
          final_map_[row][column]. pointer_to_worm_;
        final_map_[field_iterator - 1][column].
          setType_(final_map_[row][column].getType());
        final_map_[field_iterator - 1][column]. pointer_to_worm_->
          setWormPlaceRow(field_iterator - 1);
        final_map_[row][column].setType_(field.AIR);
        final_map_[row][column].setWormPointerToNull();
        return;

      }
        /// Worm drowned?
      else if((field_iterator == (getBoardHeight() - 1))
              && (final_map_[field_iterator][column].
        getType() == field.WATER))
      {
        std::cout << final_map_[row][column]. pointer_to_worm_
          ->getWormName()
                  << " (" << final_map_[row][column].
           pointer_to_worm_->getWormId()
                  << DROWNED_MESSAGE;
        final_map_[row][column]. pointer_to_worm_->setWormHp(0);
        final_map_[row][column]. pointer_to_worm_->setWormId(DEAD_ID);
        final_map_[row][column].setWormPointerToNull();
        final_map_[row][column].setType_(field.AIR);
        return;
      }
      else if((field_iterator > (row + 1))
              && (field_iterator <= getBoardHeight())
              && (final_map_[field_iterator][column].getType()
                  == field.EARTH
                  || final_map_[field_iterator][column]. pointer_to_worm_
                     != NULL))
      {
        if(final_map_[row][column]. pointer_to_worm_->getWormHp()
           <= (field_iterator - (row + 2)) * 10)
        {
          std::cout << final_map_[row][column]. pointer_to_worm_
            ->getWormName()
                    << " (" << final_map_[row][column]. pointer_to_worm_
                                                    ->getWormId()
                    << FELL_INTO_HIS_DEATH;
          final_map_[row][column]. pointer_to_worm_->setWormHp
            (-(final_map_[row][column]. pointer_to_worm_->getWormHp()));
          final_map_[row][column]. pointer_to_worm_->setWormId(DEAD_ID);
          final_map_[row][column].setType_(field.AIR);
          final_map_[row][column].setWormPointerToNull();
          return;
        }
        else
        {
          std::cout << final_map_[row][column]. pointer_to_worm_
            ->getWormName()
                    << " (" << final_map_[row][column]. pointer_to_worm_
            ->getWormId()
                    << ") took " << (field_iterator - (row + 2)) * 10
                    << "hp fall damage" << std::endl;
          final_map_[row][column]. pointer_to_worm_->
            setWormHp(-(field_iterator - (row + 2)) * 10);


          final_map_[field_iterator - 1][column]. pointer_to_worm_ =
            final_map_[row][column]. pointer_to_worm_;

          final_map_[field_iterator - 1][column].setType_
            (final_map_[row][column].getType());
          final_map_[field_iterator - 1][column]. pointer_to_worm_->
            setWormPlaceRow(field_iterator - 1);
          final_map_[row][column].setType_(field.AIR);
          final_map_[row][column].setWormPointerToNull();
          return;
        }
      }
    }
    else if(final_map_[row + 1][column].getType() == field.CHEST
             && final_map_[row][column]. pointer_to_worm_ != NULL)
    {
      wepon_number = final_map_[row + 1][column].weapon_in_chest_;
      final_map_[row + 1][column].setType_(field.AIR);
      final_map_[row + 1][column].weapon_in_chest_ = NO_WEPON;
      switch (wepon_number)
      {
        case BAZOOKA:
          final_map_[row][column]. pointer_to_worm_
            ->worm_weapon_bazooka_.setCurrentAmmunition(1);
          std::cout << final_map_[row][column]. pointer_to_worm_
            ->getWormName()
                    << " ("
                    << final_map_[row][column]. pointer_to_worm_
                      ->getWormId()
                    << WORM_PICKED_UP_BAZOOKA_MESSAGE;
          break;
        case TELEPORT:
          final_map_[row][column]. pointer_to_worm_
            ->worm_weapon_tepelport_.setCurrentAmmunition(1);
          std::cout << final_map_[row][column]. pointer_to_worm_
            ->getWormName()
                    << " ("
                    << final_map_[row][column]. pointer_to_worm_
                      ->getWormId()
                    << WORM_PICKED_UP_TELEPORT_MESSAGE;
          break;
        case BLOWTORCH:
          final_map_[row][column]. pointer_to_worm_
            ->worm_weapon_flamethrower_.setCurrentAmmunition(1);
          std::cout << final_map_[row][column]. pointer_to_worm_
            ->getWormName()
                    << " ("
                    << final_map_[row][column]. pointer_to_worm_
                      ->getWormId()
                    << WORM_PICKED_UP_BLOWTORCH_MESSAGE;
          break;
        case MELEE:
          final_map_[row][column]. pointer_to_worm_
            ->worm_weapon_melee_.setCurrentAmmunition(1);
          std::cout << final_map_[row][column]. pointer_to_worm_
            ->getWormName()
                    << " ("
                    << final_map_[row][column]. pointer_to_worm_
                      ->getWormId()
                    << WORM_PICKED_UP_MELEE_MESSAGE;
          break;
        case AIRSTRIKE:
          final_map_[row][column]. pointer_to_worm_
            ->worm_weapon_airstrike_.setCurrentAmmunition(1);
          std::cout << final_map_[row][column]. pointer_to_worm_
            ->getWormName()
                    << " ("
                    << final_map_[row][column]. pointer_to_worm_
                      ->getWormId()
                    << WORM_PICKED_UP_AIRSTRIKE_MESSAGE;
          break;
      }
    }
    row++;
  }
}


int Sep::Game::shootBazookaLeftSide(int row)
{
  int gravity_time = 0;
  if(final_map_[row][0].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << row << ", 0)"
              << std::endl;
    final_map_[row][0].setType_(field.AIR);
  }
  else if(final_map_[row][0].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << row << ", 0)"
              << std::endl;
    final_map_[row][0].setType_(field.AIR);
    final_map_[row][0].weapon_in_chest_ = NO_WEPON;
  }
  else if(final_map_[row][0]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << row
              << ", 0)" << std::endl;
    if (final_map_[row][0]. pointer_to_worm_->getWormHp() <=
      - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row][0]. pointer_to_worm_->getWormName()
                << " (" << final_map_[row][0]. pointer_to_worm_
        ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row][0]. pointer_to_worm_->setWormHp(0);
      final_map_[row][0]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row][0].setType_(field.AIR);
      final_map_[row][0].setWormPointerToNull();
    }
    else
    {
      final_map_[row][0]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row][0]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[row][0]. pointer_to_worm_
        ->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
  /// Worm is up?
  if(final_map_[row - 1][0]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << row - 1
              << ", 0)" << std::endl;
    if (final_map_[row - 1][0]. pointer_to_worm_->getWormHp() <=
      - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row - 1][0]. pointer_to_worm_->getWormName()
                << " (" << final_map_[row - 1][0]. pointer_to_worm_
        ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row - 1][0]. pointer_to_worm_->setWormHp(0);
      final_map_[row - 1][0]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row - 1][0].setType_(field.AIR);
      final_map_[row - 1][0].setWormPointerToNull();
    }
    else
    {
      final_map_[row - 1][0]. pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row - 1][0]. pointer_to_worm_->getWormName()
                << " (" << final_map_[row - 1][0]. pointer_to_worm_
        ->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
  /// Chest is up??
  else if(final_map_[row - 1][0].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << row - 1
              << ", 0)" << std::endl;
    final_map_[row - 1][0].setType_(field.AIR);
    final_map_[row - 1][0].weapon_in_chest_ = NO_WEPON;
  }
  /// Earth is up;
  else if(final_map_[row - 1][0].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << row - 1
              << ", 0)" << std::endl;
    final_map_[row - 1][0].setType_(field.AIR);
  }

  /// Worm is down?
  if(final_map_[row + 1][0]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << row + 1
              << ", 0)" << std::endl;
    if (final_map_[row + 1][0]. pointer_to_worm_->getWormHp() <=
      - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row + 1][0]. pointer_to_worm_->getWormName()
                << " (" << final_map_[row + 1][0]. pointer_to_worm_
        ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row + 1][0]. pointer_to_worm_->setWormHp(0);
      final_map_[row + 1][0]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row + 1][0].setType_(field.AIR);
      final_map_[row + 1][0].setWormPointerToNull();
    }
    else
    {
      final_map_[row + 1][0]. pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row + 1][0]. pointer_to_worm_->getWormName()
                << " (" << final_map_[row + 1][0]. pointer_to_worm_
        ->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
  /// Chest is down??
  else if(final_map_[row + 1][0].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << row + 1
              << ", 0)" << std::endl;
    final_map_[row + 1][0].setType_(field.AIR);
    final_map_[row + 1][0].weapon_in_chest_ = NO_WEPON;
  }
  /// Earth is down??
  else if(final_map_[row + 1][0].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << row + 1
              << ", 0)" << std::endl;
    final_map_[row + 1][0].setType_(field.AIR);
  }
  while(gravity_time < 20)
  {
    applyGravity(0);
    gravity_time++;
  }

  return RETURN_OK;
}


int Sep::Game::shootBazookaInMiddle(int row, int column)
{
  int gravity_time = 0;
  /// Shoot hit Earth?
  if(final_map_[row][column].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << row << ", "
              << column <<")"
              << std::endl;
    final_map_[row][column].setType_(field.AIR);
  }
  /// Shoot hit Chest?
  else if(final_map_[row][column].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << row << ", "
              << column <<")"
              << std::endl;
    final_map_[row][column].setType_(field.AIR);
    final_map_[row][column].weapon_in_chest_ = NO_WEPON;
  }
  /// Shoot hit Worm?
  else if(final_map_[row][column]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << row << ", "
              << column <<")"
              << std::endl;
    if (final_map_[row][column]. pointer_to_worm_->getWormHp() <=
      - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row][column]. pointer_to_worm_->getWormName()
                << " (" << final_map_[row][column]. pointer_to_worm_
        ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row][column]. pointer_to_worm_->setWormHp(0);
      final_map_[row][column]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row][column].setType_(field.AIR);
      final_map_[row][column].setWormPointerToNull();
    }
    else
    {
      final_map_[row][column]. pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row][column]. pointer_to_worm_->getWormName()
                << " (" << final_map_[row][column]. pointer_to_worm_
        ->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
  /// Worm is up?
  if(final_map_[row - 1][column]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << row - 1
              << ", "
              << column
              << ")"
              << std::endl;
    if (final_map_[row - 1][column]. pointer_to_worm_->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row - 1][column]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[row - 1][column]. pointer_to_worm_
        ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row - 1][column]. pointer_to_worm_->setWormHp(0);
      final_map_[row - 1][column]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row - 1][column].setType_(field.AIR);
      final_map_[row - 1][column].setWormPointerToNull();
    }
    else
    {
      final_map_[row - 1][column]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row - 1][column]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[row - 1][column]. pointer_to_worm_
        ->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is up??
  else if(final_map_[row - 1][column].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << row - 1 << ", "
              << column << ")" << std::endl;
    final_map_[row - 1][column].setType_(field.AIR);
    final_map_[row - 1][column].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is up;
  else if(final_map_[row - 1][column].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << row - 1 << ", "
              << column << ")" << std::endl;
    final_map_[row - 1][column].setType_(field.AIR);
  }
  /// Worm is right?
  if(final_map_[row][column + 1]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << row
              << ", "
              << column + 1 <<")" << std::endl;
    if (final_map_[row][column + 1]. pointer_to_worm_->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row][column + 1]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[row][column + 1]. pointer_to_worm_
        ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row][column + 1]. pointer_to_worm_->setWormHp(0);
      final_map_[row][column + 1]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row][column + 1].setType_(field.AIR);
      final_map_[row][column + 1].setWormPointerToNull();
    }
    else
    {
      final_map_[row][column + 1]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row][column + 1]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[row][column + 1]. pointer_to_worm_
        ->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is right??
  else if(final_map_[row][column + 1].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST
              << row << ", "
              << column + 1<< ")" << std::endl;
    final_map_[row][column + 1].setType_(field.AIR);
    final_map_[row][column + 1].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is right?;
  else if(final_map_[row][column + 1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << row << ", "
              << column + 1<< ")" << std::endl;
    final_map_[row][column + 1].setType_(field.AIR);
  }

  /// Worm is down?
  if(final_map_[row + 1][column]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << row + 1 << ", "
              << column <<")" << std::endl;
    if (final_map_[row + 1][column]. pointer_to_worm_->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row + 1][column]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[row + 1][column]. pointer_to_worm_
        ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row + 1][column]. pointer_to_worm_->setWormHp(0);
      final_map_[row + 1][column]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row + 1][column].setType_(field.AIR);
      final_map_[row + 1][column].setWormPointerToNull();
    }
    else
    {
      final_map_[row + 1][column]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row + 1][column]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[row + 1][column]. pointer_to_worm_
        ->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is down??
  else if(final_map_[row + 1][column].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST
              << row + 1 << ", "
              << column << ")" << std::endl;
    final_map_[row + 1][column].setType_(field.AIR);
    final_map_[row + 1][column].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is down??
  else if(final_map_[row + 1][column].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << row + 1 << ", "
              << column << ")" << std::endl;
    final_map_[row + 1][column].setType_(field.AIR);
  }

  /// Worm is left?
  if(final_map_[row][column - 1]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM<< row << ", "
              << column - 1 <<")" << std::endl;
    if (final_map_[row][column - 1]. pointer_to_worm_->getWormHp() <=
      - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row][column - 1]. pointer_to_worm_
        ->getWormName()
                << " ("
                << final_map_[row][column + 1]. pointer_to_worm_
                  ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row][column - 1]. pointer_to_worm_->setWormHp(0);
      final_map_[row][column - 1]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row][column - 1].setType_(field.AIR);
      final_map_[row][column - 1].setWormPointerToNull();
    }
    else
    {
      final_map_[row][column - 1]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row][column - 1]. pointer_to_worm_
        ->getWormName()
                << " ("
                << final_map_[row][column - 1]. pointer_to_worm_
                  ->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is left??
  else if(final_map_[row][column - 1].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << row << ", "
              << column - 1<< ")" << std::endl;
    final_map_[row][column - 1].setType_(field.AIR);
    final_map_[row][column - 1].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is left?;
  else if(final_map_[row][column - 1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << row << ", "
              << column - 1 << ")" << std::endl;
    final_map_[row][column - 1].setType_(field.AIR);
  }

  while(gravity_time < 20)
  {
    applyGravity(column);
    gravity_time++;
  }
  gravity_time = 0;
  while(gravity_time < 20)
  {
    applyGravity(column + 1);
    gravity_time++;
  }
  gravity_time = 0;
  while(gravity_time < 20)
  {
    applyGravity(column - 1);
    gravity_time++;
  }

  return RETURN_OK;

}


int Sep::Game::shootBazookaDown(int column)
{
  int gravity_time = 0;
  /// Shoot hit Earth?
  if(final_map_[getBoardHeight() - 1][column].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << getBoardHeight() - 1 << ", "
              << column <<")"
              << std::endl;
    final_map_[getBoardHeight() - 1][column].setType_(field.AIR);
  }
    /// Shoot hit Chest?
  else if(final_map_[getBoardHeight() - 1][column].getType()
          == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << getBoardHeight() - 1
              << ", "
              << column <<")"
              << std::endl;
    final_map_[getBoardHeight() - 1][column].setType_(field.AIR);
    final_map_[getBoardHeight() - 1][column].weapon_in_chest_ = NO_WEPON;
  }
    /// Shoot hit Worm?
  else if(final_map_[getBoardHeight() - 1][column]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << getBoardHeight() - 1
              << ", "
              << column <<")"
              << std::endl;
    if (final_map_[getBoardHeight() - 1][column]. pointer_to_worm_
          ->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 1][column].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1][column].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 1][column]. pointer_to_worm_
        ->setWormHp(0);
      final_map_[getBoardHeight() - 1][column]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 1][column].setType_(field.AIR);
      final_map_[getBoardHeight() - 1][column].setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 1][column]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 1][column]
        . pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[getBoardHeight() - 1][column].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }


  /// Worm is up?
  if(final_map_[getBoardHeight() - 2][column]. pointer_to_worm_
     != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << getBoardHeight() - 2 << ", "
              << column <<")" << std::endl;
    if (final_map_[getBoardHeight() - 2][column].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 2][column].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 2][column].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 2][column].
         pointer_to_worm_->setWormHp(0);
      final_map_[getBoardHeight() - 2][column].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 2][column].setType_(field.AIR);
      final_map_[getBoardHeight() - 2][column].setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 2][column].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 2][column].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 2][column].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is up??
  else if(final_map_[getBoardHeight() - 2][column].getType()
          == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << getBoardHeight() - 2
              << ", "
              << column << ")" << std::endl;
    final_map_[getBoardHeight() - 2][column].setType_(field.AIR);
    final_map_[getBoardHeight() - 2][column].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is up;
  else if(final_map_[getBoardHeight() - 2][column].getType()
          == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << getBoardHeight() - 2 << ", "
              << column << ")" << std::endl;
    final_map_[getBoardHeight() - 2][column].setType_(field.AIR);
  }
  /// Worm is right?
  if(final_map_[getBoardHeight() - 1][column + 1]. pointer_to_worm_
     != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << getBoardHeight() - 1
              << ", "
              << column + 1 <<")" << std::endl;
    if (final_map_[getBoardHeight() - 1][column + 1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 1][column + 1].
         pointer_to_worm_->getWormName()
                << " ("
                << final_map_[getBoardHeight() - 1][column + 1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 1][column + 1].
         pointer_to_worm_->setWormHp(0);
      final_map_[getBoardHeight() - 1][column + 1].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 1][column + 1].
        setType_(field.AIR);
      final_map_[getBoardHeight() - 1][column + 1].setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 1][column + 1].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 1][column + 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1][column + 1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is right??
  else if(final_map_[getBoardHeight() - 1][column + 1].getType() ==
    field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << getBoardHeight() - 1
              << ", "
              << column + 1 << ")" << std::endl;
    final_map_[getBoardHeight() - 1][column + 1].setType_(field.AIR);
    final_map_[getBoardHeight() - 1][column + 1].weapon_in_chest_
      = NO_WEPON;
  }
    /// Earth is right?;
  else if(final_map_[getBoardHeight() - 1][column + 1].getType()
          == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << getBoardHeight() - 1
              << ", "
              << column + 1 << ")" << std::endl;
    final_map_[getBoardHeight() - 1][column + 1].setType_(field.AIR);
  }

  /// Worm is left?
  if(final_map_[getBoardHeight() - 1][column - 1].
     pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << getBoardHeight() - 1 << ", "
              << column - 1 << ")" << std::endl;
    if (final_map_[getBoardHeight() - 1][column - 1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 1][column - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1][column + 1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 1][column - 1].
         pointer_to_worm_->setWormHp(0);
      final_map_[getBoardHeight() - 1][column - 1].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 1][column - 1].
        setType_(field.AIR);
      final_map_[getBoardHeight() - 1][column - 1].
        setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 1][column - 1].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 1][column - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1][column - 1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is left??
  else if(final_map_[getBoardHeight() - 1][column - 1].getType()
          == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST
              << getBoardHeight() - 1 << ", "
              << column - 1<< ")" << std::endl;
    final_map_[getBoardHeight() - 1][column - 1].
      setType_(field.AIR);
    final_map_[getBoardHeight() - 1][column - 1].
      weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is left?;
  else if(final_map_[getBoardHeight() - 1][column - 1].
    getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << getBoardHeight() - 1 << ", "
              << column - 1 << ")" << std::endl;
    final_map_[getBoardHeight() - 1][column - 1].
      setType_(field.AIR);
  }
  gravity_time = 0;
  while(gravity_time < 15)
  {
    applyGravity(column);
    gravity_time++;
  }
  gravity_time = 0;
  while(gravity_time < 15)
  {
    applyGravity(column + 1);
    gravity_time++;
  }
  gravity_time = 0;
  while(gravity_time < 15)
  {
    applyGravity(column - 1);
    gravity_time++;
  }

  return RETURN_OK;
}


int Sep::Game::shootBazookaTop(int column)
{
  /// Shoot hit Earth?
  if(final_map_[0][column].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << 0 << ", "
              << column <<")"
              << std::endl;
    final_map_[0][column].setType_(field.AIR);
  }
    /// Shoot hit Chest?
  else if(final_map_[0][column].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << 0 << ", "
              << column << ")"
              << std::endl;
    final_map_[0][column].setType_(field.AIR);
    final_map_[0][column].weapon_in_chest_ = NO_WEPON;
  }
    /// Shoot hit Worm?
  else if(final_map_[0][column]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << 0 << ", "
              << column << ")"
              << std::endl;
    if (final_map_[0][column]. pointer_to_worm_->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[0][column]. pointer_to_worm_->getWormName()
                << " (" << final_map_[0][column].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[0][column]. pointer_to_worm_->setWormHp(0);
      final_map_[0][column]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[0][column].setType_(field.AIR);
      final_map_[0][column].setWormPointerToNull();
    }
    else
    {
      final_map_[0][column]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[0][column]. pointer_to_worm_->getWormName()
                << " (" << final_map_[0][column]. pointer_to_worm_
        ->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
  /// Worm is right?
  if(final_map_[0][column + 1]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << 0 << ", "
              << column + 1 <<")" << std::endl;
    if (final_map_[0][column + 1]. pointer_to_worm_->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[0][column + 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[0][column + 1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[0][column + 1]. pointer_to_worm_->setWormHp(0);
      final_map_[0][column + 1].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[0][column + 1].setType_(field.AIR);
      final_map_[0][column + 1].setWormPointerToNull();
    }
    else
    {
      final_map_[0][column + 1]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[0][column + 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[0][column + 1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is right??
  else if(final_map_[0][column + 1].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << 0 << ", "
              << column + 1<< ")" << std::endl;
    final_map_[0][column + 1].setType_(field.AIR);
    final_map_[0][column + 1].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is right?;
  else if(final_map_[0][column + 1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << 0 << ", "
              << column + 1 << ")" << std::endl;
    final_map_[0][column + 1].setType_(field.AIR);
  }
  /// Worm is down?
  if(final_map_[1][column]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << 1
              << ", " << column << ")" << std::endl;
    if (final_map_[1][column]. pointer_to_worm_->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[1][column]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[1][column]. pointer_to_worm_
        ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[1][column]. pointer_to_worm_->setWormHp(0);
      final_map_[1][column]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[1][column].setType_(field.AIR);
      final_map_[1][column].setWormPointerToNull();
    }
    else
    {
      final_map_[1][column]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[1][column].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[1][column].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is down??
  else if(final_map_[1][column].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << 1
              << ", " << column << ")" << std::endl;
    final_map_[1][column].setType_(field.AIR);
    final_map_[1][column].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is down??
  else if(final_map_[1][column].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << 1
              << ", " << column << ")" << std::endl;
    final_map_[1][column].setType_(field.AIR);
  }
  /// Worm is left?
  if(final_map_[0][column - 1]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << 0 << ", "
              << column - 1 <<")" << std::endl;
    if (final_map_[0][column - 1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[0][column - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[0][column - 1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[0][column - 1]. pointer_to_worm_
        ->setWormHp(0);
      final_map_[0][column - 1]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[0][column - 1].setType_(field.AIR);
      final_map_[0][column - 1].setWormPointerToNull();
    }
    else
    {
      final_map_[0][column - 1].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[0][column - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[0][column - 1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is left??
  else if(final_map_[0][column - 1].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << 0
              << ", "
              << column - 1<< ")" << std::endl;
    final_map_[0][column - 1].setType_(field.AIR);
    final_map_[0][column - 1].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is left?;
  else if(final_map_[0][column - 1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << 0 << ", "
              << column - 1 << ")" << std::endl;
    final_map_[0][column - 1].setType_(field.AIR);
  }

  return RETURN_OK;
}

int Sep::Game::shootBazookaRightSide(int row)
{
  int gravity_time = 0;
  /// Shoot hit Earth?
  if(final_map_[row][getBoardWidth() - 1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << row << ", "
              << getBoardWidth() - 1 <<")"
              << std::endl;
    final_map_[row][getBoardWidth() - 1].setType_(field.AIR);
  }
    /// Shoot hit Chest?
  else if(final_map_[row][getBoardWidth() - 1].
    getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST
              << row << ", "
              << getBoardWidth() - 1 << ")"
              << std::endl;
    final_map_[row][getBoardWidth() - 1].setType_(field.AIR);
    final_map_[row][getBoardWidth() - 1].weapon_in_chest_ = NO_WEPON;
  }
    /// Shoot hit Worm?
  else if(final_map_[row][getBoardWidth() - 1].
     pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << row << ", "
              << getBoardWidth() - 1 << ")"
              << std::endl;
    if (final_map_[row][getBoardWidth() - 1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row][getBoardWidth() - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[row][getBoardWidth() - 1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row][getBoardWidth() - 1]. pointer_to_worm_
        ->setWormHp(0);
      final_map_[row][getBoardWidth() - 1]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[row][getBoardWidth() - 1].setType_(field.AIR);
      final_map_[row][getBoardWidth() - 1].setWormPointerToNull();
    }
    else
    {
      final_map_[row][getBoardWidth() - 1]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row][getBoardWidth() - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[row][getBoardWidth() - 1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
  /// Worm is up?
  if(final_map_[row - 1][getBoardWidth() - 1]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << row - 1 << ", "
              << getBoardWidth() - 1 << ")" << std::endl;
    if (final_map_[row - 1][getBoardWidth() - 1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row - 1][getBoardWidth() - 1].
           pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[row - 1]
      [getBoardWidth() - 1]. pointer_to_worm_
        ->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row - 1][getBoardWidth() - 1]. pointer_to_worm_
        ->setWormHp(0);
      final_map_[row - 1][getBoardWidth() - 1]. pointer_to_worm_
        ->setWormId(-DEAD_ID);
      final_map_[row - 1][getBoardWidth() - 1].
        setType_(field.AIR);
      final_map_[row - 1][getBoardWidth() - 1].
        setWormPointerToNull();
    }
    else
    {
      final_map_[row - 1][getBoardWidth() - 1].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row - 1][getBoardWidth() - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[row - 1][getBoardWidth() - 1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is up??
  else if(final_map_[row - 1][getBoardWidth() - 1].
    getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << row - 1 << ", "
              << getBoardWidth() - 1 << ")" << std::endl;
    final_map_[row - 1][getBoardWidth() - 1].setType_(field.AIR);
    final_map_[row - 1][getBoardWidth() - 1].weapon_in_chest_
      = NO_WEPON;
  }
    /// Earth is up;
  else if(final_map_[row - 1][getBoardWidth() - 1].
    getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << row - 1
              << ", "
              << getBoardWidth() - 1 << ")"
              << std::endl;
    final_map_[row - 1][getBoardWidth() - 1].
      setType_(field.AIR);
  }
  /// Worm is down?
  if(final_map_[row + 1][getBoardWidth() - 1]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << row + 1 << ", "
              << getBoardWidth() - 1 <<")" << std::endl;
    if (final_map_[row + 1][getBoardWidth() - 1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row + 1][getBoardWidth() - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[row + 1][getBoardWidth() - 1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row + 1][getBoardWidth() - 1].
         pointer_to_worm_->setWormHp(0);
      final_map_[row + 1][getBoardWidth() - 1].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row + 1][getBoardWidth() - 1].
        setType_(field.AIR);
      final_map_[row + 1][getBoardWidth() - 1].
        setWormPointerToNull();
    }
    else
    {
      final_map_[row + 1][getBoardWidth() - 1].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row + 1][getBoardWidth() - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[row + 1][getBoardWidth() - 1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is down??
  else if(final_map_[row + 1][getBoardWidth() - 1].getType()
          == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST
              << row + 1 << ", "
              << getBoardWidth() - 1 << ")"
              << std::endl;
    final_map_[row + 1][getBoardWidth() - 1]
      .setType_(field.AIR);
    final_map_[row + 1][getBoardWidth() - 1]
      .weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is down??
  else if(final_map_[row + 1][getBoardWidth() - 1].
    getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << row + 1 << ", "
              << getBoardWidth() - 1 << ")" << std::endl;
    final_map_[row + 1][getBoardWidth() - 1].setType_(field.AIR);
  }
  /// Worm is left?
  if(final_map_[row][getBoardWidth() - 2].
     pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << row << ", "
              << getBoardWidth() - 2 <<")"
              << std::endl;
    if (final_map_[row][getBoardWidth() - 2].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[row][getBoardWidth() - 2].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[row][getBoardWidth() - 2].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[row][getBoardWidth() - 2].
         pointer_to_worm_->setWormHp(0);
      final_map_[row][getBoardWidth() - 2].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[row][getBoardWidth() - 2].
        setType_(field.AIR);
      final_map_[row][getBoardWidth() - 2].
        setWormPointerToNull();
    }
    else
    {
      final_map_[row][getBoardWidth() - 2].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[row][getBoardWidth() - 2].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[row][getBoardWidth() - 2].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is left??
  else if(final_map_[row][getBoardWidth() - 2].
    getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST
              << row << ", "
              << getBoardWidth() - 2 << ")" << std::endl;
    final_map_[row][getBoardWidth() - 2].
      setType_(field.AIR);
    final_map_[row][getBoardWidth() - 2].
      weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is left?;
  else if(final_map_[row][getBoardWidth() - 2].
    getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << row << ", "
              << getBoardWidth() - 2 << ")"
              << std::endl;
    final_map_[row][getBoardWidth() - 2].
      setType_(field.AIR);
  }
  while(gravity_time < 20)
  {
    applyGravity(getBoardWidth() - 1);
    gravity_time++;
  }
  gravity_time = 0;
  while(gravity_time < 20)
  {
    applyGravity(getBoardWidth() - 2);
    gravity_time++;
  }

  return RETURN_OK;
}

int Sep::Game::shootBazookaTopLeftCorner()
{
  int gravity_time = 0;
  /// Shoot hit Earth?
  if(final_map_[0][0].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << 0 << ", "
              << 0 <<")"
              << std::endl;
    final_map_[0][0].setType_(field.AIR);
  }
    /// Shoot hit Chest?
  else if(final_map_[0][0].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << 0 << ", "
              << 0 <<")"
              << std::endl;
    final_map_[0][0].setType_(field.AIR);
    final_map_[0][0].weapon_in_chest_ = NO_WEPON;
  }
    /// Shoot hit Worm?
  else if(final_map_[0][0]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << 0
              << ", "
              << 0 <<")"
              << std::endl;
    if (final_map_[0][0]. pointer_to_worm_->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[0][0]. pointer_to_worm_->getWormName()
                << " (" << final_map_[0][0]. pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[0][0]. pointer_to_worm_->setWormHp(0);
      final_map_[0][0]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[0][0].setType_(field.AIR);
      final_map_[0][0].setWormPointerToNull();
    }
    else
    {
      final_map_[0][0]. pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[0][0]. pointer_to_worm_->getWormName()
                << " (" << final_map_[0][0]. pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
  /// Worm is right?
  if(final_map_[0][1]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << 0 << ", "
              << 1 <<")" << std::endl;
    if (final_map_[0][1]. pointer_to_worm_->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[0][1]. pointer_to_worm_->getWormName()
                << " (" << final_map_[0][1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[0][1]. pointer_to_worm_->setWormHp(0);
      final_map_[0][1]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[0][1].setType_(field.AIR);
      final_map_[0][1].setWormPointerToNull();
    }
    else
    {
      final_map_[0][1]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[0][1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[0][1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is right??
  else if(final_map_[0][1].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST
              << 0 << ", "
              << 1 << ")" << std::endl;
    final_map_[0][1].setType_(field.AIR);
    final_map_[0][1].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is right?;
  else if(final_map_[0][1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << 0 << ", "
              << 1 << ")" << std::endl;
    final_map_[0][1].setType_(field.AIR);
  }
  /// Worm is down?
  if(final_map_[1][0]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << 1
              << ", " << 0 << ")" << std::endl;
    if (final_map_[1][0]. pointer_to_worm_->getWormHp() <=
      - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[1][0]. pointer_to_worm_->getWormName()
                << " (" << final_map_[1][0]. pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[1][0]. pointer_to_worm_->setWormHp(0);
      final_map_[1][0]. pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[1][0].setType_(field.AIR);
      final_map_[1][0].setWormPointerToNull();
    }
    else
    {
      final_map_[1][0]. pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[1][0]. pointer_to_worm_->getWormName()
                << " (" << final_map_[1][0]. pointer_to_worm_->
        getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is down??
  else if(final_map_[1][0].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << 1
              << ", " << 0 << ")" << std::endl;
    final_map_[1][0].setType_(field.AIR);
    final_map_[1][0].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is down??
  else if(final_map_[1][0].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << 1
              << ", " << 0 << ")" << std::endl;
    final_map_[1][0].setType_(field.AIR);
  }
  while(gravity_time < 20)
  {
    applyGravity(0);
    gravity_time++;
  }
  return RETURN_OK;
}

int Sep::Game::shootBazookaTopRightCorner()
{
  int gravity_time = 0;
  /// Shoot hit Earth?
  if(final_map_[0][getBoardWidth() - 1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << 0 << ", "
              << getBoardWidth() - 1 <<")"
              << std::endl;
    final_map_[0][getBoardWidth() - 1].setType_(field.AIR);
  }
    /// Shoot hit Chest?
  else if(final_map_[0][getBoardWidth() - 1].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << 0 << ", "
              << getBoardWidth() - 1 <<")"
              << std::endl;
    final_map_[0][getBoardWidth() - 1].setType_(field.AIR);
    final_map_[0][getBoardWidth() - 1].weapon_in_chest_ = NO_WEPON;
  }
    /// Shoot hit Worm?
  else if(final_map_[0][getBoardWidth() - 1].
     pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << 0
              << ", "
              << getBoardWidth() - 1 << ")"
              << std::endl;
    if (final_map_[0][getBoardWidth() - 1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[0][getBoardWidth() - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[0][getBoardWidth() - 1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[0][getBoardWidth() - 1].
         pointer_to_worm_->setWormHp(0);
      final_map_[0][getBoardWidth() - 1].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[0][getBoardWidth() - 1].
        setType_(field.AIR);
      final_map_[0][getBoardWidth() - 1].
        setWormPointerToNull();
    }
    else
    {
      final_map_[0][getBoardWidth() - 1].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[0][getBoardWidth() - 1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[0][getBoardWidth() - 1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
  /// Worm is down?
  if(final_map_[1][getBoardWidth() - 1]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << 1
              << ", " << getBoardWidth() - 1 << ")" << std::endl;
    if (final_map_[1][getBoardWidth() - 1]. pointer_to_worm_
          ->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[1][getBoardWidth() - 1]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[1][getBoardWidth() - 1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[1][getBoardWidth() - 1]. pointer_to_worm_
        ->setWormHp(0);
      final_map_[1][getBoardWidth() - 1]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[1][getBoardWidth() - 1].setType_(field.AIR);
      final_map_[1][getBoardWidth() - 1].setWormPointerToNull();
    }
    else
    {
      final_map_[1][getBoardWidth() - 1]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[1][getBoardWidth() - 1]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[1][getBoardWidth() - 1]
        . pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is down??
  else if(final_map_[1][getBoardWidth() - 1].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << 1
              << ", " << getBoardWidth() - 1 << ")" << std::endl;
    final_map_[1][getBoardWidth() - 1].setType_(field.AIR);
    final_map_[1][getBoardWidth() - 1].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is down??
  else if(final_map_[1][getBoardWidth() - 1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << 1
              << ", " << getBoardWidth() - 1 << ")" << std::endl;
    final_map_[1][getBoardWidth() - 1].setType_(field.AIR);
  }
  /// Worm is left?
  if(final_map_[0][getBoardWidth() - 2]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << 0 << ", "
              << getBoardWidth() - 2 <<")" << std::endl;
    if (final_map_[0][getBoardWidth() - 2]. pointer_to_worm_
          ->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[0][getBoardWidth() - 2]. pointer_to_worm_
        ->getWormName()
                << " (" << final_map_[0][getBoardWidth() - 2].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[0][getBoardWidth() - 2]. pointer_to_worm_->setWormHp(0);
      final_map_[0][getBoardWidth() - 2]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[0][getBoardWidth() - 2].setType_(field.AIR);
      final_map_[0][getBoardWidth() - 2].setWormPointerToNull();
    }
    else
    {
      final_map_[0][getBoardWidth() - 2]. pointer_to_worm_
        ->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[0][getBoardWidth() - 2].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[0][getBoardWidth() - 2].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is left??
  else if(final_map_[0][getBoardWidth() - 2].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << 0 << ", "
              << getBoardWidth() - 2 << ")" << std::endl;
    final_map_[0][getBoardWidth() - 2].setType_(field.AIR);
    final_map_[0][getBoardWidth() - 2].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is left?;
  else if(final_map_[0][getBoardWidth() - 2].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << 0 << ", "
              << getBoardWidth() - 2 << ")" << std::endl;
    final_map_[0][getBoardWidth() - 2].setType_(field.AIR);
  }
  while(gravity_time < 20)
  {
    applyGravity(getBoardWidth() - 1);
    gravity_time++;
  }
  return RETURN_OK;
}

int Sep::Game::shootBazookaDownLeftCorner()
{
  int gravity_time = 0;
  /// Shoot hit Earth?
  if(final_map_[getBoardHeight() - 1][0].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << getBoardHeight() - 1
              << ", "
              << 0 <<")"
              << std::endl;
    final_map_[getBoardHeight() - 1][0].setType_(field.AIR);
  }
    /// Shoot hit Chest?
  else if(final_map_[getBoardHeight() - 1][0].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST
              << getBoardHeight() - 1 << ", "
              << 0 <<")"
              << std::endl;
    final_map_[getBoardHeight() - 1][0].setType_(field.AIR);
    final_map_[getBoardHeight() - 1][0].weapon_in_chest_ = NO_WEPON;
  }
    /// Shoot hit Worm?
  else if(final_map_[getBoardHeight() - 1][0]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << getBoardHeight() - 1
              << ", "
              << 0 <<")"
              << std::endl;
    if (final_map_[getBoardHeight() - 1][0]. pointer_to_worm_->getWormHp()
        <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 1][0].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1][0].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 1][0].
         pointer_to_worm_->setWormHp(0);
      final_map_[getBoardHeight() - 1][0].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 1][0].setType_(field.AIR);
      final_map_[getBoardHeight() - 1][0].setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 1][0].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 1][0].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1][0].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
  /// Worm is up?
  if(final_map_[getBoardHeight() - 2][0]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << getBoardHeight() - 2 << ", "
              << 0 <<")" << std::endl;
    if (final_map_[getBoardHeight() - 2][0].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 2][0].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 2][0].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 2][0]. pointer_to_worm_->setWormHp(0);
      final_map_[getBoardHeight() - 2][0]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 2][0].setType_(field.AIR);
      final_map_[getBoardHeight() - 2][0].setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 2][0].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 2][0].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 2][0].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is up??
  else if(final_map_[getBoardHeight() - 2][0].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST
              << getBoardHeight() - 2 << ", "
              << 0 << ")" << std::endl;
    final_map_[getBoardHeight() - 2][0].setType_(field.AIR);
    final_map_[getBoardHeight() - 2][0].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is up;
  else if(final_map_[getBoardHeight() - 2][0].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << getBoardHeight() - 2 << ", "
              << 0 << ")" << std::endl;
    final_map_[getBoardHeight() - 2][0].setType_(field.AIR);
  }
  /// Worm is right?
  if(final_map_[getBoardHeight() - 1][1]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << getBoardHeight() - 1 << ", "
              << 1 <<")" << std::endl;
    if (final_map_[getBoardHeight() - 1][1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 1][1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1][1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 1][1]. pointer_to_worm_->setWormHp(0);
      final_map_[getBoardHeight() - 1][1]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 1][1].setType_(field.AIR);
      final_map_[getBoardHeight() - 1][1].setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 1][1].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 1][1].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1][1].
         pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is right??
  else if(final_map_[getBoardHeight() - 1][1].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << getBoardHeight() - 1
              << ", "
              << 1 << ")" << std::endl;
    final_map_[getBoardHeight() - 1][1].setType_(field.AIR);
    final_map_[getBoardHeight() - 1][1].weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is right?;
  else if(final_map_[getBoardHeight() - 1][1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << getBoardHeight() - 1
              << ", "
              << 1<< ")" << std::endl;
    final_map_[getBoardHeight() - 1][1].setType_(field.AIR);
  }
  while(gravity_time < 30)
  {
    applyGravity(0);
    gravity_time++;
  }
  gravity_time = 0;
  while(gravity_time < 30)
  {
    applyGravity(1);
    gravity_time++;
  }
  return RETURN_OK;
}

int Sep::Game::shootBazookaDownRightCorner()
{
  int gravity_time = 0;
  /// Shoot hit Earth?
  if(final_map_[getBoardHeight() - 1][getBoardWidth() - 1].getType()
     == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << getBoardHeight() - 1 << ", "
              << getBoardWidth() - 1 << ")"
              << std::endl;
    final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
      setType_(field.AIR);
  }
    /// Shoot hit Chest?
  else if(final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
    getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << getBoardHeight() - 1
              << ", "
              << getBoardWidth() - 1 << ")"
              << std::endl;
    final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
      setType_(field.AIR);
    final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
      weapon_in_chest_ = NO_WEPON;
  }
    /// Shoot hit Worm?
  else if(final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
     pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << getBoardHeight() - 1 << ", "
              << getBoardWidth() - 1 << ")"
              << std::endl;
    if (final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 1][getBoardWidth() - 1]
        . pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1]
      [getBoardWidth() - 1].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
         pointer_to_worm_->setWormHp(0);
      final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
        setType_(field.AIR);
      final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
        setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 1][getBoardWidth() - 1].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 1][getBoardWidth() - 1]
        . pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1]
      [getBoardWidth() - 1]. pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }

  /// Worm is up?
  if(final_map_[getBoardHeight() - 2][getBoardWidth() - 1].
     pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << getBoardHeight() - 2
              << ", "
              << getBoardWidth() - 1 << ")" << std::endl;
    if (final_map_[getBoardHeight() - 2][getBoardWidth() - 1].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 2][getBoardWidth() - 1]
        . pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 2]
      [getBoardWidth() - 1]. pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 2][getBoardWidth() - 1].
         pointer_to_worm_->setWormHp(0);
      final_map_[getBoardHeight() - 2][getBoardWidth() - 1].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 2][getBoardWidth() - 1].
        setType_(field.AIR);
      final_map_[getBoardHeight() - 2][getBoardWidth() - 1].
        setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 2][getBoardWidth() - 1].
         pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 2][getBoardWidth() - 1]
        . pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 2]
      [getBoardWidth() - 1]. pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is up??
  else if(final_map_[getBoardHeight() - 2]
          [getBoardWidth() - 1].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << getBoardHeight() - 2
              << ", "
              << 0 << ")" << std::endl;
    final_map_[getBoardHeight() - 2][getBoardWidth() - 1].
      setType_(field.AIR);
    final_map_[getBoardHeight() - 2][getBoardWidth() - 1].
      weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is up;
  else if(final_map_[getBoardHeight() - 2]
          [getBoardWidth() - 1].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << getBoardHeight() - 2 << ", "
              << getBoardWidth() - 1 << ")" << std::endl;
    final_map_[getBoardHeight() - 2]
    [getBoardWidth() - 1].setType_(field.AIR);
  }
  /// Worm is left?
  if(final_map_[getBoardHeight() - 1]
     [getBoardWidth() - 2]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM
              << getBoardHeight() - 1 << ", "
              << getBoardWidth() - 2 << ")" << std::endl;
    if (final_map_[getBoardHeight() - 1][getBoardWidth() - 2].
       pointer_to_worm_->getWormHp() <= - BAZOOKA_DAMAGE)
    {
      std::cout << final_map_[getBoardHeight() - 1][getBoardWidth() - 2]
        . pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1]
      [getBoardWidth() - 2]. pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[getBoardHeight() - 1][getBoardWidth() - 2].
         pointer_to_worm_->setWormHp(0);
      final_map_[getBoardHeight() - 1][getBoardWidth() - 2].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[getBoardHeight() - 1][getBoardWidth() - 2].
        setType_(field.AIR);
      final_map_[getBoardHeight() - 1][getBoardWidth() - 2].
        setWormPointerToNull();
    }
    else
    {
      final_map_[getBoardHeight() - 1][getBoardWidth() - 2]
        . pointer_to_worm_->setWormHp(BAZOOKA_DAMAGE);
      std::cout << final_map_[getBoardHeight() - 1][getBoardWidth() - 2]
        . pointer_to_worm_->getWormName()
                << " (" << final_map_[getBoardHeight() - 1]
      [getBoardWidth() - 2]. pointer_to_worm_->getWormId()
                << ") took 40hp damage" << std::endl;
    }
  }
    /// Chest is left??
  else if(final_map_[getBoardHeight() - 1][getBoardWidth() - 2].
    getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << getBoardHeight() - 1
              << ", "
              << getBoardWidth() - 2 << ")" << std::endl;
    final_map_[getBoardHeight() - 1][getBoardWidth() - 2].
      setType_(field.AIR);
    final_map_[getBoardHeight() - 1][getBoardWidth() - 2].
      weapon_in_chest_ = NO_WEPON;
  }
    /// Earth is left?;
  else if(final_map_[getBoardHeight() - 1][getBoardWidth() - 2]
            .getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH
              << getBoardHeight() - 1 << ", "
              << getBoardWidth() - 2 << ")"
              << std::endl;
    final_map_[getBoardHeight() - 1]
    [getBoardWidth() - 2].setType_(field.AIR);
  }
  while(gravity_time < 30)
  {
    applyGravity(getBoardWidth() - 1);
    gravity_time++;
  }
  gravity_time = 0;
  while(gravity_time < 30)
  {
    applyGravity(getBoardWidth() - 2);
    gravity_time++;
  }
  return RETURN_OK;
}

void Sep::Game::findTarget(std::string direction, int &target_row,
                           int &target_column, int worm_in_row,
                           int worm_in_col)
{
  int trajectory;
  if(direction == "l")
    trajectory = LEFT;
  else if(direction == "r")
    trajectory = RIGHT;
  else if(direction == "d")
    trajectory = DOWN;
  else if(direction == "u")
    trajectory = UP;
  else if(direction == "d")
    trajectory = DOWN;
  else if(direction == "lu")
    trajectory = LEFT_UP;
  else if(direction == "ru")
    trajectory = RIGHT_UP;
  else if(direction == "ld")
    trajectory = LEFT_DOWN;
  else if(direction == "rd")
    trajectory = RIGHT_DOWN;


  switch (trajectory)
  {
    case LEFT :
      if(worm_in_col == 0)
      {
        target_row = - 1;
        target_column = - 1;
        return;
      }
      else if(worm_in_col == 1)
      {
        target_row = worm_in_row;
        target_column = 0;
        return;
      }
      worm_in_col--;
      while(final_map_[worm_in_row][worm_in_col].getType() == field.AIR
            && worm_in_col > 0)
      {
        worm_in_col--;
      }
      target_column = worm_in_col;
      if(final_map_[worm_in_row][target_column].getType() == field.AIR)
      {
        target_column = - 1;
        target_row = - 1;
        return;
      }
      else
      {
        target_row = worm_in_row;
        return;
      }
      break;

    case RIGHT:
      if(worm_in_col == (getBoardWidth() - 1))
      {
        target_row = - 1;
        target_column = - 1;
        return;
      }
      else if(worm_in_col == (getBoardWidth() - 2))
      {
        target_row = worm_in_row;
        target_column = (getBoardWidth() - 1);
        return;
      }
      worm_in_col++;
      while(final_map_[worm_in_row][worm_in_col].getType() == field.AIR
            && worm_in_col < (getBoardWidth() - 1))
      {
        worm_in_col++;
      }
      target_column = worm_in_col;
      if(final_map_[worm_in_row][target_column].getType() == field.AIR)
      {
        target_column = - 1;
        target_row = - 1;
        return;
      }
      else
      {
        target_row = worm_in_row;
        return;
      }
      break;

    case DOWN:
      if(worm_in_row == (getBoardHeight() - 1))
      {
        target_row = - 1;
        target_column = - 1;
        return;
      }
      else if(worm_in_row == (getBoardHeight() - 2))
      {
        target_column = worm_in_col;
        target_row = (getBoardHeight() - 1);
        return;
      }
      worm_in_row++;
      while(final_map_[worm_in_row][worm_in_col].getType() == field.AIR
            && worm_in_row < (getBoardHeight() - 1))
      {
        worm_in_row++;
      }
      target_row = worm_in_row;
      if(final_map_[target_row][worm_in_col].getType() == field.AIR)
      {
        target_column = - 1;
        target_row = - 1;
        return;
      }
      else
      {
        target_column = worm_in_col;
        return;
      }
      break;

    case UP:
      if(worm_in_row == 0)
      {
        target_row = - 1;
        target_column = - 1;
        return;
      }
      else if(worm_in_row == 1)
      {
        target_column = worm_in_col;
        target_row = 0;
        if(final_map_[target_row][target_column].getType() == field.AIR)
        {
          target_column = - 1;
          target_row = - 1;
        }
        return;
      }
      worm_in_row--;
      while(final_map_[worm_in_row][worm_in_col].getType() == field.AIR
            && worm_in_row > 0)
      {
        worm_in_row--;
      }
      target_row = worm_in_row;
      if(final_map_[target_row][worm_in_col].getType() == field.AIR)
      {
        target_column = - 1;
        target_row = - 1;
        return;
      }
      else
      {
        target_column = worm_in_col;
        return;
      }
      break;

    case LEFT_UP:
      if(worm_in_col == 0 || worm_in_row == 0)
      {
        target_row = - 1;
        target_column = - 1;
        return;
      }
      else if(worm_in_row == 1 && worm_in_col == 1)
      {
        if(final_map_[0][0].getType() == field.AIR)
        {
          target_row = - 1;
          target_column = - 1;
          return;
        }
        else
        {
          target_row = 1;
          target_column = 1;
          return;
        }
      }
      worm_in_row--;
      worm_in_col--;
      while(final_map_[worm_in_row][worm_in_col].getType() == field.AIR
            && (worm_in_row > 0 && worm_in_col > 0))
      {
        worm_in_row--;
        worm_in_col--;
      }
      target_row = worm_in_row;
      target_column = worm_in_col;
      if(final_map_[target_row][target_column].getType() == field.AIR)
      {
        target_column = - 1;
        target_row = - 1;
        return;
      }
      else
        return;
      break;

    case RIGHT_UP:
      if(worm_in_col == (getBoardWidth() - 1) || worm_in_row == 0)
      {
        target_row = - 1;
        target_column = - 1;
        return;
      }
      else if(worm_in_row == 1 && worm_in_col == (getBoardWidth() - 2))
      {
        if(final_map_[0][getBoardWidth() - 1].getType() == field.AIR)
        {
          target_row = - 1;
          target_column = - 1;
          return;
        }
        else
        {
          target_row = 1;
          target_column = (getBoardHeight() - 1);
          return;
        }
      }
      worm_in_row--;
      worm_in_col++;
      while(final_map_[worm_in_row][worm_in_col].getType() == field.AIR
            && (worm_in_row > 0 && (worm_in_col < (getBoardWidth() - 1))))
      {
        worm_in_row--;
        worm_in_col++;
      }
      target_row = worm_in_row;
      target_column = worm_in_col;
      if(final_map_[target_row][target_column].getType() == field.AIR)
      {
        target_column = - 1;
        target_row = - 1;
        return;
      }
      else
        return;
      break;

    case LEFT_DOWN:
      if(worm_in_col == 0)
      {
        target_row = - 1;
        target_column = - 1;
        return;
      }
      else if(worm_in_row == (getBoardHeight() - 2))
      {
        if(final_map_[getBoardHeight() - 1][worm_in_col - 1].
          getType() == field.AIR)
        {
          target_row = - 1;
          target_column = - 1;
          return;
        }
        else
        {
          target_row = getBoardHeight() - 1;
          target_column = worm_in_col - 1;
          return;
        }
      }
      worm_in_row++;
      worm_in_col--;
      while(final_map_[worm_in_row][worm_in_col].getType() == field.AIR
            && (worm_in_row < (getBoardHeight() - 1)
                && (worm_in_col > 0)))
      {
        worm_in_row++;
        worm_in_col--;
      }
      target_row = worm_in_row;
      target_column = worm_in_col;
      if(final_map_[target_row][target_column].getType() == field.AIR)
      {
        target_column = - 1;
        target_row = - 1;
        return;
      }
      else
        return;
      break;

    case RIGHT_DOWN:
      if(worm_in_col == (getBoardWidth() - 1))
      {
        target_row = - 1;
        target_column = - 1;
        return;
      }
      else if(worm_in_row == (getBoardHeight() - 2))
      {
        if(final_map_[getBoardHeight() - 1][worm_in_col + 1].
          getType() == field.AIR)
        {
          target_row = - 1;
          target_column = - 1;
          return;
        }
        else
        {
          target_row = getBoardHeight() - 1;
          target_column = worm_in_col + 1;
          return;
        }
      }
      worm_in_row++;
      worm_in_col++;
      while(final_map_[worm_in_row][worm_in_col].getType() == field.AIR
            && (worm_in_row < (getBoardHeight() - 1)
                && (worm_in_col < (getBoardWidth() - 1))))
      {
        worm_in_row++;
        worm_in_col++;
      }
      target_row = worm_in_row;
      target_column = worm_in_col;
      if(final_map_[target_row][target_column].getType() == field.AIR)
      {
        target_column = - 1;
        target_row = - 1;
        return;
      }
      else
        return;
      break;
  }
  return;
}



int Sep::Game::shootBazooka(Worm &worm, std::string direction)
{
  int worm_in_row = worm.getWormPlaceRow();
  int worm_in_column = worm.getWormPlaceCol();
  int target_row;
  int target_col;

  findTarget(direction, target_row, target_col, worm_in_row,
             worm_in_column);

  if((target_row == - 1) && (target_col == - 1))
  {
    std::cout << MESSAGE_SHOT_MISSED;
    return RETURN_INFO_SHOT_MISSED;
  }
  else if((target_row > 0 && target_row < (getBoardHeight() - 1))
          && (target_col == 0))
  {
    shootBazookaLeftSide(target_row);
  }
  else if((target_row > 0 && (target_row < (getBoardHeight() - 1)))
          && (target_col == (getBoardWidth() - 1)))
  {
    shootBazookaRightSide(target_row);
  }
  else if(target_row == 0
          && (target_col > 0 && (target_col < (getBoardWidth() - 1))))
  {
    shootBazookaTop(target_col);
  }
  else if(target_row == (getBoardHeight() - 1)
          && (target_col > 0 && (target_col < (getBoardWidth() - 1))))
  {
    shootBazookaDown(target_col);
  }
  else if((target_row == 0) && (target_col == 0))
  {
    shootBazookaTopLeftCorner();
  }
  else if(target_row == 0 && (target_col == getBoardWidth() - 1))
  {
    shootBazookaTopRightCorner();
  }
  else if((target_row == (getBoardHeight() - 1))
          && (target_col == 0))
  {
    shootBazookaDownLeftCorner();
  }
  else if((target_row == (getBoardHeight() - 1))
          && (target_col == (getBoardWidth() - 1)))
  {
    shootBazookaDownRightCorner();
  }
  else if((target_row > 0 && target_row < (getBoardHeight() - 1))
          && (target_col > 0 && target_col < (getBoardWidth() - 1)))
  {
    shootBazookaInMiddle(target_row, target_col);
  }


  return RETURN_OK;
}

int Sep::Game::shootGunField(int target_row, int target_column)
{
  int gravity_time = 0;
  /// Shoot hit Earth?
  if(final_map_[target_row][target_column].getType() == field.EARTH)
  {
    std::cout << MESSAGE_SHOT_HIT_EARTH << target_row << ", "
              << target_column <<")"
              << std::endl;
    final_map_[target_row][target_column].setType_(field.AIR);
  }
    /// Shoot hit Chest?
  else if(final_map_[target_row][target_column].getType() == field.CHEST)
  {
    std::cout << MESSAGE_SHOT_HIT_CHEST << target_row << ", "
              << target_column <<")"
              << std::endl;
    final_map_[target_row][target_column].setType_(field.AIR);
    final_map_[target_row][target_column].weapon_in_chest_ = NO_WEPON;
  }
    /// Shoot hit Worm?
  else if(final_map_[target_row][target_column]. pointer_to_worm_ != NULL)
  {
    std::cout << MESSAGE_SHOT_HIT_WORM << target_row << ", "
              << target_column << ")"
              << std::endl;
    if (final_map_[target_row]
        [target_column]. pointer_to_worm_->getWormHp() <= - GUN_DAMAGE)
    {
      std::cout << final_map_[target_row]
      [target_column]. pointer_to_worm_->getWormName()
                << " (" << final_map_[target_row]
      [target_column]. pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[target_row][target_column]. pointer_to_worm_->setWormHp(0);
      final_map_[target_row][target_column]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[target_row][target_column].setType_(field.AIR);
      final_map_[target_row][target_column].setWormPointerToNull();
    }
    else
    {
      final_map_[target_row][target_column].
         pointer_to_worm_->setWormHp(GUN_DAMAGE);
      std::cout << final_map_[target_row][target_column].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[target_row][target_column].
         pointer_to_worm_->getWormId()
                << ") took 25hp damage" << std::endl;
    }
  }
  gravity_time = 0;
  while(gravity_time < 20)
  {
    applyGravity(target_column);
    gravity_time++;
  }

  return RETURN_OK;
}

int Sep::Game::shootGun(Worm &worm, std::string direction)
{
  int worm_in_row = worm.getWormPlaceRow();
  int worm_in_column = worm.getWormPlaceCol();
  int target_row;
  int target_col;

  findTarget(direction, target_row, target_col, worm_in_row,
             worm_in_column);

  if((target_row == - 1) && (target_col == - 1))
  {
    std::cout << MESSAGE_SHOT_MISSED;
    return RETURN_INFO_SHOT_MISSED;
  }
  else
  {
    shootGunField(target_row, target_col);
    return RETURN_OK;
  }

}

int Sep::Game::shootFlameOneField(int target_row, int target_column)
{
  /// Shoot hit Earth?
  if(final_map_[target_row][target_column].getType() == field.EARTH)
  {
    std::cout << "Torch hit Earth at position (" << target_row << ", "
              << target_column <<")"
              << std::endl;
    final_map_[target_row][target_column].setType_(field.AIR);
  }
    /// Shoot hit Chest?
  else if(final_map_[target_row][target_column].getType() == field.CHEST)
  {
    std::cout << "Torch hit Chest at position (" << target_row << ", "
              << target_column <<")"
              << std::endl;
    final_map_[target_row][target_column].setType_(field.AIR);
    final_map_[target_row][target_column].weapon_in_chest_ = NO_WEPON;
  }
    /// Shoot hit Worm?
  else if(final_map_[target_row][target_column]. pointer_to_worm_ != NULL)
  {
    std::cout << "Torch hit Worm at position (" << target_row << ", "
              << target_column <<")"
              << std::endl;
    if (final_map_[target_row][target_column]. pointer_to_worm_->getWormHp()
        <= -FLAMETHROWER_DAMAGE)
    {
      std::cout << final_map_[target_row][target_column].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[target_row][target_column].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[target_row][target_column]. pointer_to_worm_
        ->setWormHp(0);
      final_map_[target_row][target_column]. pointer_to_worm_
        ->setWormId(DEAD_ID);
      final_map_[target_row][target_column].setType_(field.AIR);
      final_map_[target_row][target_column].setWormPointerToNull();
    }
    else
    {
      final_map_[target_row][target_column]. pointer_to_worm_
        ->setWormHp(FLAMETHROWER_DAMAGE);
      std::cout << final_map_[target_row][target_column].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[target_row][target_column].
         pointer_to_worm_->getWormId()
                << ") took 35hp damage" << std::endl;
    }
  }
  return RETURN_OK;
}

int Sep::Game::shootBlowtorch(Worm &worm, std::string direction)
{
  int map_iterator = 0;
  int column_iterator = 0;
  int gravity_time = 0;
  int use_of_gravity = 0;
  if(direction == "l")
  {
    if(worm.getWormPlaceCol() == 0)
      return RETURN_OK;
    else
    {
      if(worm.getWormPlaceCol() == 1)
      {
        shootFlameOneField(worm.getWormPlaceRow(),
                           worm.getWormPlaceCol() - 1);
        while(gravity_time < 20)
        {
          applyGravity(0);
          gravity_time++;
        }
        return RETURN_OK;
      }
      while((worm.getWormPlaceCol() - column_iterator != 0)
            && (map_iterator <= 4))
      {
        map_iterator++;
        shootFlameOneField(worm.getWormPlaceRow(),
                           worm.getWormPlaceCol() - map_iterator);
        column_iterator++;
      }
      gravity_time = 0;
      map_iterator = 0;
      while(column_iterator > 0)
      {
        map_iterator++;
        while(gravity_time < 20)
        {
          applyGravity(worm.getWormPlaceCol() - map_iterator);
          gravity_time++;
        }
        gravity_time = 0;
        column_iterator--;
      }
      return RETURN_OK;
    }
  }
  else if(direction == "r")
  {
    if(worm.getWormPlaceCol() == (getBoardWidth() - 1))
      return RETURN_OK;
    else
    {
      if(worm.getWormPlaceCol() == (getBoardWidth() - 2))
      {
        shootFlameOneField(worm.getWormPlaceRow(),
                           worm.getWormPlaceCol() + 1);
        while(gravity_time < 20)
        {
          applyGravity(getBoardWidth() - 1);
          gravity_time++;
        }
        return RETURN_OK;
      }
      while((worm.getWormPlaceCol() + column_iterator
             != (getBoardWidth() - 1))
            && (map_iterator <= 4))
      {
        map_iterator++;
        shootFlameOneField(worm.getWormPlaceRow(),
                           worm.getWormPlaceCol() + map_iterator);
        column_iterator++;
      }
      gravity_time = 0;
      column_iterator = 0;
      while(column_iterator < map_iterator)
      {
        column_iterator++;
        while(gravity_time < 20)
        {
          applyGravity(worm.getWormPlaceCol() + column_iterator);
          gravity_time++;
        }
        gravity_time = 0;
      }
      return RETURN_OK;
    }
  }
  else if(direction == "u")
  {
    if(worm.getWormPlaceRow() == 0)
      return RETURN_OK;
    else
    {
      if(worm.getWormPlaceRow() == 1)
      {
        shootFlameOneField(worm.getWormPlaceRow() - 1,
                           worm.getWormPlaceCol());
        while(gravity_time < 20)
        {
          applyGravity(worm.getWormPlaceCol());
          gravity_time++;
        }
        return RETURN_OK;
      }
      while((worm.getWormPlaceRow() - column_iterator != 0)
            && (map_iterator <= 4))
      {
        map_iterator++;
        shootFlameOneField(worm.getWormPlaceRow() - map_iterator,
                           worm.getWormPlaceCol());
        column_iterator++;
      }
      gravity_time = 0;
      while(gravity_time < 20)
      {
        applyGravity(worm.getWormPlaceCol());
        gravity_time++;
      }
      return RETURN_OK;
    }
  }
  else if(direction == "d")
  {
    if(worm.getWormPlaceRow() == (getBoardHeight() - 1))
      return RETURN_OK;
    else
    {
      if(worm.getWormPlaceRow() == (getBoardHeight() - 2))
      {
        shootFlameOneField(worm.getWormPlaceRow() + 1,
                           worm.getWormPlaceCol());
        while(gravity_time < 20)
        {
          applyGravity(worm.getWormPlaceCol());
          gravity_time++;
        }
        return RETURN_OK;
      }
      while((worm.getWormPlaceRow() + column_iterator
             != (getBoardHeight() - 1))
            && (map_iterator <= 4))
      {
        map_iterator++;
        shootFlameOneField(worm.getWormPlaceRow() + map_iterator,
                           worm.getWormPlaceCol());
        column_iterator++;
      }
      gravity_time = 0;
      while(gravity_time < 20)
      {
        applyGravity(worm.getWormPlaceCol());
        gravity_time++;
      }
      return RETURN_OK;
    }
  }
  else if(direction == "lu")
  {
    column_iterator = worm.getWormPlaceCol();
    gravity_time = worm.getWormPlaceRow();
    map_iterator = 0;
    while((column_iterator > 0) && (gravity_time > 0)
          && (map_iterator < 5))
    {
      map_iterator++;
      column_iterator--;
      gravity_time--;
      shootFlameOneField(gravity_time, column_iterator);
      while(use_of_gravity < 20)
      {
        applyGravity(column_iterator);
        use_of_gravity++;
      }
      use_of_gravity = 0;
    }
    return RETURN_OK;
  }
  else if(direction == "ld")
  {
    column_iterator = worm.getWormPlaceCol();
    gravity_time = worm.getWormPlaceRow();
    map_iterator = 0;
    while((column_iterator > 0) && (gravity_time < (getBoardHeight() - 1))
          && (map_iterator < 5))
    {
      map_iterator++;
      column_iterator--;
      gravity_time++;
      shootFlameOneField(gravity_time, column_iterator);
      while(use_of_gravity < 20)
      {
        applyGravity(column_iterator);
        use_of_gravity++;
      }
      use_of_gravity = 0;
    }
    return RETURN_OK;
  }
  else if(direction == "ru")
  {
    column_iterator = worm.getWormPlaceCol();
    gravity_time = worm.getWormPlaceRow();
    map_iterator = 0;
    while((column_iterator < (getBoardWidth() - 1) && (gravity_time > 0))
          && (map_iterator < 5))
    {
      map_iterator++;
      column_iterator++;
      gravity_time--;
      shootFlameOneField(gravity_time, column_iterator);
      while(use_of_gravity < 20)
      {
        applyGravity(column_iterator);
        use_of_gravity++;
      }
      use_of_gravity = 0;
    }
    return RETURN_OK;
  }
  else if(direction == "rd")
  {
    column_iterator = worm.getWormPlaceCol();
    gravity_time = worm.getWormPlaceRow();
    map_iterator = 0;
    while((column_iterator < (getBoardWidth() - 1))
          && (gravity_time < (getBoardHeight() - 1))
          && (map_iterator < 5))
    {
      map_iterator++;
      column_iterator++;
      gravity_time++;
      shootFlameOneField(gravity_time, column_iterator);
      while(use_of_gravity < 20)
      {
        applyGravity(column_iterator);
        use_of_gravity++;
      }
      use_of_gravity = 0;
    }
    return RETURN_OK;
  }
  return RETURN_OK;
}

int Sep::Game::shootMeleOneField(int target_row, int target_column)
{
  /// Shoot hit Worm?
  if(final_map_[target_row][target_column]. pointer_to_worm_ != NULL)
  {
    std::cout << "Attack hit Worm at position (" << target_row << ", "
              << target_column << ")"
              << std::endl;
    if (final_map_[target_row][target_column]. pointer_to_worm_
          ->getWormHp()
        <= - MELEE_DAMAGE)
    {
      std::cout << final_map_[target_row][target_column].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[target_row][target_column].
         pointer_to_worm_->getWormId()
                << MESSAGE_WORM_DIED << std::endl;
      final_map_[target_row][target_column].
         pointer_to_worm_->setWormHp(0);
      final_map_[target_row][target_column].
         pointer_to_worm_->setWormId(DEAD_ID);
      final_map_[target_row][target_column].
        setType_(field.AIR);
      final_map_[target_row][target_column].
        setWormPointerToNull();
    }
    else
    {
      final_map_[target_row][target_column].
         pointer_to_worm_->setWormHp(MELEE_DAMAGE);
      std::cout << final_map_[target_row][target_column].
         pointer_to_worm_->getWormName()
                << " (" << final_map_[target_row]
      [target_column]. pointer_to_worm_->getWormId()
                << ") took 50hp damage" << std::endl;
    }
  }

  return RETURN_OK;
}

bool Sep::Game::inField(int row, int column)
{
  if( row < 0 || row >= getBoardHeight())
    return false;
  if( column < 0 || column >= getBoardWidth())
    return false;
  return true;
}

int Sep::Game::actionMelee(Worm &worm)
{
  const int row_offset[] = { - 1, - 1, 0,  1, 1, 1,  0, - 1 };// 8 shifts to neighbors
  const int column_offset[] = { 0,  1,  1, 1, 0, - 1, - 1, - 1 };// used in functions
  const int neighborhood = 8;

  int row_target = worm.getWormPlaceRow();
  int column_target = worm.getWormPlaceCol();
  int offset_iterator;
  int gravity_time = 0;
  for(offset_iterator = 0; offset_iterator < neighborhood;
      ++offset_iterator)// visit the 8 spaces around it
    if( inField(row_target + row_offset[offset_iterator], column_target +
      column_offset[offset_iterator]))
    {
      shootMeleOneField(row_target + row_offset[offset_iterator],
                        column_target + column_offset[offset_iterator]);
      while(gravity_time < 20)
      {
        applyGravity(column_target);
        gravity_time++;
      }
    }

  return RETURN_OK;
}