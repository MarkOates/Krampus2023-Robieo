

#include <Pipeline/Testing/Comparison/Pipeline/GameProgressAndStateInfo.hpp>


namespace Pipeline
{


bool operator==(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object)
{
   if (object.get_delivered_package_identifiers() != other_object.get_delivered_package_identifiers()) return false;
   return true;
}


void PrintTo(const GameProgressAndStateInfo& object, ::std::ostream* os)
{
   //*os << "GameProgressAndStateInfo(" << object.x << ", " << object.y << ")";
}


}


