//-----------------------------------------------------------------------------
// Blowtorch.cpp
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
//-----------------------------------------------------------------------------
//
#include "Blowtorch.h"


Sep::Blowtorch::Blowtorch(): weapon_name_("blowtorch"), ammunition_(1),
                             damage_to_enemy_(35) {};

std::string Sep::Blowtorch::getWeaponName()
{
  return weapon_name_;
}

unsigned long int Sep::Blowtorch::getCurrentAmmunition()
{
  return ammunition_;
}

void Sep::Blowtorch::setCurrentAmmunition(int add_ammunition)
{
  ammunition_ += add_ammunition;
}