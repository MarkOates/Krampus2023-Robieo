#pragma once


#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <LabyrinthOfLore/Physics/TileFaceEnum.hpp>


namespace LabyrinthOfLore
{
   namespace Physics
   {
      class EntityTileMapCollisionEvent
      {
      private:
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* entity;
         int tile_type;
         int tile_x;
         int tile_y;
         LabyrinthOfLore::WorldMap::tile_face_t tile_face_collided_with;
         float force;

      protected:


      public:
         EntityTileMapCollisionEvent(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* entity=nullptr, int tile_type=0, int tile_x=0, int tile_y=0, LabyrinthOfLore::WorldMap::tile_face_t tile_face_collided_with=LabyrinthOfLore::WorldMap::TILE_FACE_NONE, float force=0);
         ~EntityTileMapCollisionEvent();

         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* get_entity() const;
         int get_tile_type() const;
         int get_tile_x() const;
         int get_tile_y() const;
         LabyrinthOfLore::WorldMap::tile_face_t get_tile_face_collided_with() const;
         float get_force() const;
      };
   }
}



