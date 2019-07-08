//-----------------------------------------------------------------------------
// Airstrike.cpp
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
//  Implemented getter and setter functions for variables:
//   -weapon_name_
//   -ammunition_
//-----------------------------------------------------------------------------
//
#include "Airstrike.h"

Sep::Airstrike::Airstrike(): weapon_name_("airstrike"), ammunition_(1),
                             damage_to_enemy_(20) {};

std::string Sep::Airstrike::getWeaponName()
{
  return weapon_name_;
}

unsigned long int Sep::Airstrike::getCurrentAmmunition()
{
  return ammunition_;
}

void Sep::Airstrike::setCurrentAmmunition(int add_ammunition)
{
  ammunition_ += add_ammunition;
}
