//-----------------------------------------------------------------------------
// Teleport.h
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
// Functions described in Weapon.h
//-----------------------------------------------------------------------------
//

#ifndef TELEPORT_H
#define TELEPORT_H

#include "Weapon.h"

namespace Sep
{
  class Teleport : public Weapon
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
    Teleport();

    
    std::string getWeaponName() override;

    
    unsigned long int getCurrentAmmunition() override;

    
    void setCurrentAmmunition(int add_ammunition) override;



  };
}

#endif //SECOND_TRY_TELEPORT_H
