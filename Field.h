//-----------------------------------------------------------------------------
// Field.h
//
// Group: Group 21815, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
// Field class is used to describe type of fields in game sworms.
// Field can be Water, Air and Earth. They also may represent
// worms of Team1(type Worm) and Team2(Worm2).
// One field can also be a chest(Chest) containing weapon ammunition
// which is later added to that worm who first gets that chest.
//-----------------------------------------------------------------------------
//
#ifndef FIELD_H
#define FIELD_H
#include <string>
#include "Worm.h"

namespace Sep
{
  class Field
  {
  public:
    enum FieldType {AIR, EARTH, WATER, WORM, WORM2, CHEST};

    //------------------------------------------------------------------------------
    /// Constructor for class Field, sets each type of field on AIR;
    ///
    //
    Field(); /// Class constructor

    //------------------------------------------------------------------------------
    /// This function sets type_ (setter)
    ///
    ///
    /// @param field_type
    //
    void setType_(FieldType field_type);

    //------------------------------------------------------------------------------
    /// This function sets the pointer to worm on NULL
    //
    void setWormPointerToNull();

    //------------------------------------------------------------------------------
    /// This function sets on worm
    /// @param worm_object ist the worm on which adress should be pointed
    //
    void setPointerOnWorm(Worm worm_object);

    //------------------------------------------------------------------------------
    /// Getter for type_ variable in private part of this class.
    /// @param field variable of type enum "FieldType".
    /// @return type_ from private part of class.
    //
    FieldType getType_(FieldType field)
    {
      return type_;
    };

    //------------------------------------------------------------------------------
    /// Function returns enum variable for given char in input.
    /// Function getEnum is used in Game.cpp after map(made of letters A, W, E)
    /// is loaded into string vector.
    ///
    /// @param input letter.
    /// @return 'AIR'   if input-letter is 'A'
    ///         'WATER' if input-letter is 'W'
    ///         'EARTH' if input-letter is 'E'
    ///
    //
    FieldType getEnum(char input)
    {
      switch (input)
      {
        case 'A':
          return AIR;
        case 'W':
          return WATER;
        case 'E':
          return EARTH;
      }
      return AIR;
    };

    //------------------------------------------------------------------------------
    /// Functions returns FieldType char;
    ///
    /// @param type given type of field at that place. It can be: AIR, EARTH, WATER
    ///             or WORM.
    /// @return ' ' (space) for FieldType AIR,
    ///         'E' for FieldType EARTH
    ///         'W' for FieldType WATER
    ///         '~' for FieldType WORM
    //
    char getCharacter(FieldType type_);

    static const int no_weapon_in_chest_ = -1;
    //------------------------------------------------------------------------------
    /// Getter function
    /// @return pointer to worm
    //
    Worm *getPointerToWorm();

    //------------------------------------------------------------------------------
    /// Getter function
    /// @return fieldtype
    //
    FieldType  getType()
    {
      return type_;
    }

    /// Variables should from be changed from other files:
    Worm *pointer_to_worm_ = NULL;
    int weapon_in_chest_;

  private:
    FieldType type_;
  };
}


#endif //FIELD_H
