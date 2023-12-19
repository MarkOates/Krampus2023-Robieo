#pragma once


#include <Pipeline/GameProgressAndStateInfo.hpp>
#include <ostream>


namespace Pipeline
{
   bool operator==(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object);
   void PrintTo(const GameProgressAndStateInfo& object, ::std::ostream* os);
}


