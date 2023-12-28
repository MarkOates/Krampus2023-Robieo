

#include <Pipeline/Gameplay/LevelCameraZone.hpp>




namespace Pipeline
{
namespace Gameplay
{


LevelCameraZone::LevelCameraZone(std::string name, AllegroFlare::Physics::AABB3D bounding_box)
   : name(name)
   , bounding_box(bounding_box)
{
}


LevelCameraZone::~LevelCameraZone()
{
}


void LevelCameraZone::set_name(std::string name)
{
   this->name = name;
}


void LevelCameraZone::set_bounding_box(AllegroFlare::Physics::AABB3D bounding_box)
{
   this->bounding_box = bounding_box;
}


std::string LevelCameraZone::get_name() const
{
   return name;
}


AllegroFlare::Physics::AABB3D LevelCameraZone::get_bounding_box() const
{
   return bounding_box;
}


AllegroFlare::Physics::AABB3D &LevelCameraZone::get_bounding_box_ref()
{
   return bounding_box;
}




} // namespace Gameplay
} // namespace Pipeline


