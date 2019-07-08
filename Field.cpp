//------------------------------------------------------------------------------
// Field.cpp
//
// Group: Group 12, study assistant Michael Hancianu
//
// Authors: Ilhan Skender, 1531567
//          Haris Hadzimahmutovic, 01634756
//
//------------------------------------------------------------------------------
//
#include "Field.h"

using namespace Sep;

Sep::Field::Field()
{
  type_ = AIR;
  weapon_in_chest_ = no_weapon_in_chest_;
}

void Sep::Field::setType_(FieldType field_type)
{
  Field::type_ = field_type;
}

char Sep::Field::getCharacter(FieldType type)
{
  if(type == 0)
    return ' ';
  else if(type == 1)
    return 'E';
  else if(type == 2)
    return 'W';
  else if(type == 3)
    return '~';
  else if(type == 4)
    return '*';
  else if(type == 5)
    return '#';
  else return ' ';
}

void Sep::Field::setPointerOnWorm(Worm worm_object)
{
  pointer_to_worm_ = &worm_object;
}


Worm* Sep::Field::getPointerToWorm()
{
  return pointer_to_worm_;
}

void Sep::Field::setWormPointerToNull()
{
  pointer_to_worm_ = NULL;
}

