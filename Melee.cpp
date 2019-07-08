//-----------------------------------------------------------------------------
// Melee.cpp
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
//-----------------------------------------------------------------------------
//
#include "Melee.h"

Sep::Melee::Melee(): weapon_name_("melee"), ammunition_(1),
                 damage_to_enemy_(50) {};

std::string Sep::Melee::getWeaponName()
{
  return weapon_name_;
}

unsigned long int Sep::Melee::getCurrentAmmunition()
{
  return ammunition_;
}

void Sep::Melee::setCurrentAmmunition(int add_ammunition)
{
  ammunition_ += add_ammunition;
}