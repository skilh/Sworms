//-----------------------------------------------------------------------------
// Worm.h
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
// 
// Apstract class, used to build classes: Gun, Bazooka, Teleport, Blowtorch and Airstrike.
//-----------------------------------------------------------------------------
//

#ifndef WEAPON_H
#define WEAPON_H
#include <string>

namespace Sep
{

  class Weapon
  {
  private:

  public:


    //------------------------------------------------------------------------------
    /// set ammunition to some big number;
    //
    virtual unsigned long int getCurrentAmmunition() = 0;


    //------------------------------------------------------------------------------
    // functions return name of weapon
    // @return name of weapon - Gun;
    //
    virtual std::string getWeaponName() = 0;



    //------------------------------------------------------------------------------
    //
    // @return number of bullets left in gun
    // 
    virtual void setCurrentAmmunition(int add_ammunition) = 0;



  };
}

#endif //SECOND_TRY_WEAPON_H
