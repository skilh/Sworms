//-----------------------------------------------------------------------------
// Teleport.cpp
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
//-----------------------------------------------------------------------------
//
#include "Teleport.h"



Sep::Teleport::Teleport(): weapon_name_("teleport"), ammunition_(1),
                 damage_to_enemy_(0) {};

std::string Sep::Teleport::getWeaponName()
{
  return weapon_name_;
}

unsigned long int Sep::Teleport::getCurrentAmmunition()
{
  return ammunition_;
}

void Sep::Teleport::setCurrentAmmunition(int add_ammunition)
{
  ammunition_ += add_ammunition;
}
