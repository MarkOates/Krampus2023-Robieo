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
         LabyrinthOfLore::WorldMap::TileMap* tile_map;
         std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities;
         static float floor_height;
         static float auto_ascend_threshold;
         static float offset_at_collision_edge;
         std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> events_from_last_processed_step;

      protected:


      public:
         EntityTileMapCollisionStepper(LabyrinthOfLore::WorldMap::TileMap* tile_map=nullptr, std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities={});
         ~EntityTileMapCollisionStepper();

         void set_tile_map(LabyrinthOfLore::WorldMap::TileMap* tile_map);
         void set_entities(std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities);
         LabyrinthOfLore::WorldMap::TileMap* get_tile_map() const;
         std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> get_entities() const;
         static float get_floor_height();
         static float get_auto_ascend_threshold();
         static float get_offset_at_collision_edge();
         std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> get_events_from_last_processed_step() const;
         void process_step();
      };
   }
}



