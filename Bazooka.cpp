//-----------------------------------------------------------------------------
// Bazooka.cpp
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
//-----------------------------------------------------------------------------
//
#include "Bazooka.h"

Sep::Bazooka::Bazooka(): weapon_name_("bazooka"), ammunition_(1),
                         damage_to_enemy_(40) {};

std::string Sep::Bazooka::getWeaponName()
{
  return weapon_name_;
}

unsigned long int Sep::Bazooka::getCurrentAmmunition()
{
  return ammunition_;
}

void Sep::Bazooka::setCurrentAmmunition(int add_ammunition)
{
  ammunition_ += add_ammunition;
}



