

#include <Pipeline/Gameplay/LevelSwitchPlateZone.hpp>




namespace Pipeline
{
namespace Gameplay
{


LevelSwitchPlateZone::LevelSwitchPlateZone(std::string name, AllegroFlare::Physics::AABB3D bounding_box)
   : name(name)
   , bounding_box(bounding_box)
{
}


LevelSwitchPlateZone::~LevelSwitchPlateZone()
{
}


void LevelSwitchPlateZone::set_name(std::string name)
{
   this->name = name;
}


void LevelSwitchPlateZone::set_bounding_box(AllegroFlare::Physics::AABB3D bounding_box)
{
   this->bounding_box = bounding_box;
}


std::string LevelSwitchPlateZone::get_name() const
{
   return name;
}


AllegroFlare::Physics::AABB3D LevelSwitchPlateZone::get_bounding_box() const
{
   return bounding_box;
}


AllegroFlare::Physics::AABB3D &LevelSwitchPlateZone::get_bounding_box_ref()
{
   return bounding_box;
}




} // namespace Gameplay
} // namespace Pipeline


