

#include <Pipeline/Testing/Comparison/Pipeline/GameProgressAndStateInfo.hpp>


namespace Pipeline
{


bool operator==(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object)
{
   if (object.get_delivered_package_identifiers() != other_object.get_delivered_package_identifiers()) return false;
   return true;
}


bool operator!=(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object)
{
   return !(object == other_object);
}


void PrintTo(const GameProgressAndStateInfo& object, ::std::ostream* os)
{
   *os << "Pipeline::GameProgressAndStateInfo(";
   *os << "FadeToWhite::GameProgressAndStateInfo(";

   // TODO: Improve thid output dump to include the key
   //*os << "last_entered_chapter_identifier: \"" << object.get_last_entered_chapter_identifier() << "\", ";

   // TODO: Improve thid output dump to include the key
   *os << "delivered_package_identifiers: { ";
   for (auto &item : object.get_delivered_package_identifiers())
   {
      *os << "\"" << item << "\", ";
   }
   *os << "}, ";

   *os << ")";
}


}


