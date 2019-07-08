//-----------------------------------------------------------------------------
// Melee.h
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
// Functions are described in Weapon.h
//-----------------------------------------------------------------------------
//

#ifndef MELEE_H
#define MELEE_H

#include "Weapon.h"


namespace Sep
{
  class Melee : public Weapon
  {
  protected:

  private:
    std::string weapon_name_;
    unsigned long int ammunition_;
    int damage_to_enemy_;


  public:

    //------------------------------------------------------------------------------
    /// Constructs new instace of class Gun;
    //
    Melee();

    
    std::string getWeaponName() override;

    
    unsigned long int getCurrentAmmunition() override;

    
    void setCurrentAmmunition(int add_ammunition) override;


  };
}


#endif //SECOND_TRY_MELEE_H
