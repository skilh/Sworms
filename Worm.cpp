//-----------------------------------------------------------------------------
// Worm.cpp
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
//-----------------------------------------------------------------------------
//
#include "Worm.h"
#include <sstream>
namespace Sep
{
  Sep::Worm::Worm(): worm_hp_(100), worm_name_(),
                     current_weapon_("gun"),
                     worm_id_(0), worm_place_row_(0),
                     worm_place_col_(), team_number_(),
                     order_(0), worm_weapon_bazooka_(),
                     worm_weapon_gun_(),
                     worm_weapon_airstrike_(),
                     worm_weapon_flamethrower_(),
                     worm_weapon_melee_(),
                     worm_weapon_tepelport_() {};

  void Sep::Worm::setWormHp(int new_hp)
  {
    worm_hp_ += new_hp;
  }

  int Sep::Worm::getWormHp()
  {
    return worm_hp_;
  }

  void Sep::Worm::setWormName(std::string name)
  {
    worm_name_ = name;
  }

  std::string Sep::Worm::getWormName()
  {
    return worm_name_;
  }

  void Sep::Worm::setWormId(int new_id)
  {
    worm_id_ = new_id;
  }

  int Sep::Worm::getWormId()
  {
    return worm_id_;
  }

  void Sep::Worm::setWormPlaceRow(int new_row)
  {
    worm_place_row_ = new_row;
  }

  int Sep::Worm::getWormPlaceRow()
  {
    return worm_place_row_;
  }

  void Sep::Worm::setWormPlaceCol(int new_col)
  {
    worm_place_col_ = new_col;
  }

  int Sep::Worm::getWormPlaceCol()
  {
    return worm_place_col_;
  }

  void Sep::Worm::setWormInTeam(int new_team_number)
  {
    team_number_ = new_team_number;
  }

  int Sep::Worm::getWormTeamNumber()
  {
    return team_number_;
  }

  std::string Sep::Worm::setCurrentWeapon(std::string
   new_weapon)
  {
    std::stringstream is(new_weapon);
    is >> current_weapon_;
    return current_weapon_;
  }

  std::string Sep::Worm::getCurrentWeapon()
  {
    return current_weapon_;
  }
}


 
