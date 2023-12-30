

#include <Pipeline/Gameplay/LevelSwitchPlateZone.hpp>




namespace Pipeline
{
namespace Gameplay
{


LevelSwitchPlateZone::LevelSwitchPlateZone(std::string name, AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* switch_entity, AllegroFlare::Physics::AABB3D bounding_box)
   : name(name)
   , switch_entity(switch_entity)
   , bounding_box(bounding_box)
   , is_activated(false)
{
}


LevelSwitchPlateZone::~LevelSwitchPlateZone()
{
}


void LevelSwitchPlateZone::set_name(std::string name)
{
   this->name = name;
}


void LevelSwitchPlateZone::set_switch_entity(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* switch_entity)
{
   this->switch_entity = switch_entity;
}


void LevelSwitchPlateZone::set_bounding_box(AllegroFlare::Physics::AABB3D bounding_box)
{
   this->bounding_box = bounding_box;
}


void LevelSwitchPlateZone::set_is_activated(bool is_activated)
{
   this->is_activated = is_activated;
}


std::string LevelSwitchPlateZone::get_name() const
{
   return name;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* LevelSwitchPlateZone::get_switch_entity() const
{
   return switch_entity;
}


AllegroFlare::Physics::AABB3D LevelSwitchPlateZone::get_bounding_box() const
{
   return bounding_box;
}


bool LevelSwitchPlateZone::get_is_activated() const
{
   return is_activated;
}


AllegroFlare::Physics::AABB3D &LevelSwitchPlateZone::get_bounding_box_ref()
{
   return bounding_box;
}




} // namespace Gameplay
} // namespace Pipeline


