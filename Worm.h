//-----------------------------------------------------------------------------
// Worm.h
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
//-----------------------------------------------------------------------------
//

#ifndef WORM_H
#define WORM_H
#include <string>
#include <vector>
#include "Bazooka.h"
#include "Airstrike.h"
#include "Blowtorch.h"
#include "Gun.h"
#include "Melee.h"
#include "Teleport.h"
#include "Airstrike.h"

namespace Sep
{
  class Worm
  {
  private:
    int worm_hp_;
    std::string worm_name_;
    std::string current_weapon_;
    int worm_id_;
    int worm_place_row_;
    int worm_place_col_;
    int team_number_;

  public:

    //------------------------------------------------------------------------------
    /// Sets new value for worm_hp
    /// @param new_hp is negativ number when worm looses health points.
    //
    void setWormHp(int new_hp);

    //------------------------------------------------------------------------------
    /// Each worm has it's own name. This function is used to set private member
    /// worm_name to some specific string(e.g. name)
    /// @param name new name for worm.
    //
    void setWormName(std::string name);

    //------------------------------------------------------------------------------
    /// Each worm has it's own ID.
    /// Team 1 has IDs [1-3] and
    //  Team 2 has IDS [4-6]
    /// @param new_id for worm.
    //
    void setWormId(int new_id);

    //------------------------------------------------------------------------------
    /// To keep track of each worm in map, we need to save
    /// it's place(row and col).
    /// This function is called each time when worm has
    /// moved to new place and update private member place_row.
    /// @param new_row
    //
    void setWormPlaceRow(int new_row);

    //------------------------------------------------------------------------------
    /// Same as row, column is updated with this function.
    /// @param new_col
    //
    void setWormPlaceCol(int new_col);

    //------------------------------------------------------------------------------
    /// Team 1 and Team 2 are only two possible teams in this game.
    /// Each worm will be added to one of these two teams.
    /// @param new_team_number
    //
    void setWormInTeam(int new_team_number);


    /// Variable should be modified in other files (main.cpp and Game.cpp)
    int order_;

    //------------------------------------------------------------------------------
    ///
    /// @return 1 or 2, depending on team;
    //
    int getWormTeamNumber();


    std::string setCurrentWeapon(std::string new_weapon);

    //------------------------------------------------------------------------------
    ///
    /// @return current column in of the worm.
    //
    int getWormPlaceCol();

    //------------------------------------------------------------------------------
    ///
    /// @return current health points of worm
    //
    int getWormHp();

    //------------------------------------------------------------------------------
    /// Getter for std::string worm_name
    /// @return worm's name.
    //
    std::string getWormName();

    //------------------------------------------------------------------------------
    ///
    /// @return worm's ID number.
    //
    int getWormId();


    //------------------------------------------------------------------------------
    ///
    /// @return current row place of worm in map.
    //
    int getWormPlaceRow();



    //------------------------------------------------------------------------------
    ///
    /// Function returns current wepon saved in string.
    /// It can be either bazooka, teleport, blowtorch, gun or melee.
    ///
    ///
    /// @return
    //
    std::string getCurrentWeapon();
    
    Bazooka worm_weapon_bazooka_;

    Gun worm_weapon_gun_;

    Airstrike worm_weapon_airstrike_;

    Blowtorch worm_weapon_flamethrower_;

    Melee worm_weapon_melee_;

    Teleport worm_weapon_tepelport_;

    /// Constructor for class worm.
    Worm();

  };
}


#endif //SECOND_TRY_WORM_H
