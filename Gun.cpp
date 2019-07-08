//-----------------------------------------------------------------------------
// Gun.cpp
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
//-----------------------------------------------------------------------------
//


#include "Gun.h"


Sep::Gun::Gun(): weapon_name_("gun"), ammunition_(42949672),
                 damage_to_enemy_(25) {};

std::string Sep::Gun::getWeaponName()
{
  return weapon_name_;
}

unsigned long int Sep::Gun::getCurrentAmmunition()
{
  return ammunition_;
}

void Sep::Gun::setCurrentAmmunition(int add_ammunition)
{

}

