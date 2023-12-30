#pragma once


#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/Physics/AABB3D.hpp>
#include <string>


namespace Pipeline
{
   namespace Gameplay
   {
      class LevelSwitchPlateZone
      {
      private:
         std::string name;
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* switch_entity;
         AllegroFlare::Physics::AABB3D bounding_box;
         bool is_activated;

      protected:


      public:
         LevelSwitchPlateZone(std::string name="[unset-name]", AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* switch_entity=nullptr, AllegroFlare::Physics::AABB3D bounding_box={});
         ~LevelSwitchPlateZone();

         void set_name(std::string name);
         void set_switch_entity(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* switch_entity);
         void set_bounding_box(AllegroFlare::Physics::AABB3D bounding_box);
         void set_is_activated(bool is_activated);
         std::string get_name() const;
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* get_switch_entity() const;
         AllegroFlare::Physics::AABB3D get_bounding_box() const;
         bool get_is_activated() const;
         AllegroFlare::Physics::AABB3D &get_bounding_box_ref();
      };
   }
}



