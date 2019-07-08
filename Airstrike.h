//-----------------------------------------------------------------------------
// Airstrike.h
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
// Functions are described in Weapon.h
//-----------------------------------------------------------------------------
//
#ifndef AIRSTRIKE_H
#define AIRSTRIKE_H
#include "Weapon.h"

namespace Sep
{
  class Airstrike : public Weapon
  {
  protected:

  private:
    std::string weapon_name_;
    unsigned long int ammunition_;
    int damage_to_enemy_;


  public:

    //------------------------------------------------------------------------------
    // Constructs new instace of class Airstrike;
    //
    Airstrike();

    
    std::string getWeaponName() override;

    
    unsigned long int getCurrentAmmunition() override;

    
    void setCurrentAmmunition(int add_ammunition) override;

  };
}


#endif //SECOND_TRY_AIRSTRIKE_H
