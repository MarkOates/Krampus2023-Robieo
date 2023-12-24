#pragma once


#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <vector>


namespace Pipeline
{
   namespace Physics
   {
      class GravityStepper
      {
      private:
         std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities;
         AllegroFlare::vec3d gravity;

      protected:


      public:
         GravityStepper(std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities={});
         ~GravityStepper();

         void set_entities(std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities);
         void set_gravity(AllegroFlare::vec3d gravity);
         std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> get_entities() const;
         AllegroFlare::vec3d get_gravity() const;
         void process_step();
      };
   }
}



