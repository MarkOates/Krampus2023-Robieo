

#include <Pipeline/Physics/GravityStepper.hpp>




namespace Pipeline
{
namespace Physics
{


GravityStepper::GravityStepper(std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities)
   : entities(entities)
   , gravity(0.0f, 0.0f, -0.003f)
{
}


GravityStepper::~GravityStepper()
{
}


void GravityStepper::set_entities(std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities)
{
   this->entities = entities;
}


void GravityStepper::set_gravity(AllegroFlare::vec3d gravity)
{
   this->gravity = gravity;
}


std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> GravityStepper::get_entities() const
{
   return entities;
}


AllegroFlare::vec3d GravityStepper::get_gravity() const
{
   return gravity;
}


void GravityStepper::process_step()
{
   for (auto &entity : entities)
   {
      entity->get_velocity_ref().position += gravity;
   }
   return;
}


} // namespace Physics
} // namespace Pipeline


