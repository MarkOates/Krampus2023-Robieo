

#include <Pipeline/Gameplay/LevelCameraZone.hpp>




namespace Pipeline
{
namespace Gameplay
{


LevelCameraZone::LevelCameraZone(AllegroFlare::Physics::AABB3D bounding_box)
   : bounding_box(bounding_box)
{
}


LevelCameraZone::~LevelCameraZone()
{
}


void LevelCameraZone::set_bounding_box(AllegroFlare::Physics::AABB3D bounding_box)
{
   this->bounding_box = bounding_box;
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


