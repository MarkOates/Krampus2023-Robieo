#pragma once


#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <LabyrinthOfLore/Physics/EntityTileMapCollisionEvent.hpp>
#include <LabyrinthOfLore/WorldMap/TileMap.hpp>
#include <vector>


namespace LabyrinthOfLore
{
   namespace Physics
   {
      class EntityTileMapCollisionStepper
      {
      private:
         static LabyrinthOfLore::WorldMap::TileMap dummy_tile_map;
         LabyrinthOfLore::WorldMap::TileMap& tile_map;
         std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities;
         float ceiling_height;
         static float floor_height;
         static float auto_ascend_threshold;
         static float offset_at_collision_edge;
         std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> events_from_last_processed_step;

      protected:


      public:
         EntityTileMapCollisionStepper(LabyrinthOfLore::WorldMap::TileMap& tile_map=get_dummy_tile_map(), std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities={});
         ~EntityTileMapCollisionStepper();

         float get_ceiling_height() const;
         static float get_floor_height();
         static float get_auto_ascend_threshold();
         static float get_offset_at_collision_edge();
         std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> get_events_from_last_processed_step() const;
         static LabyrinthOfLore::WorldMap::TileMap& get_dummy_tile_map();
         void process_step();
      };
   }
}



