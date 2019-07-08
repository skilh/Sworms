//-----------------------------------------------------------------------------
// Bazooka.h
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//  
// Functions are decribed in Weapon.h
//-----------------------------------------------------------------------------
//
#ifndef BAZOOKA_H
#define BAZOOKA_H

#include "Weapon.h"


namespace Sep
{
  class Bazooka : public Weapon
  {
  private:
    std::string weapon_name_;
    unsigned long int ammunition_;
    int damage_to_enemy_;


  public:

    //------------------------------------------------------------------------------
    /// Constructs new instace of class Bazooka;
    //
    Bazooka();

    
    std::string getWeaponName() override ;

    
    unsigned long int getCurrentAmmunition() override ;

    
    void setCurrentAmmunition(int add_ammunition) override ;
  };
}

#endif