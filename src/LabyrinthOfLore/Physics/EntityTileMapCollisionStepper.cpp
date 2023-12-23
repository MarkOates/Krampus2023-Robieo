

#include <LabyrinthOfLore/Physics/EntityTileMapCollisionStepper.hpp>

#include <LabyrinthOfLore/Physics/EntityTileMapCollisionEvent.hpp>
#include <LabyrinthOfLore/WorldMap/TileFaceEnum.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace LabyrinthOfLore
{
namespace Physics
{


float EntityTileMapCollisionStepper::floor_height = 0.0f;


float EntityTileMapCollisionStepper::auto_ascend_threshold = 0.25f;


float EntityTileMapCollisionStepper::offset_at_collision_edge = 0.001f;


EntityTileMapCollisionStepper::EntityTileMapCollisionStepper(LabyrinthOfLore::WorldMap::TileMap* tile_map, std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities)
   : tile_map(tile_map)
   , entities(entities)
   , events_from_last_processed_step({})
{
}


EntityTileMapCollisionStepper::~EntityTileMapCollisionStepper()
{
}


void EntityTileMapCollisionStepper::set_tile_map(LabyrinthOfLore::WorldMap::TileMap* tile_map)
{
   this->tile_map = tile_map;
}


void EntityTileMapCollisionStepper::set_entities(std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities)
{
   this->entities = entities;
}


LabyrinthOfLore::WorldMap::TileMap* EntityTileMapCollisionStepper::get_tile_map() const
{
   return tile_map;
}


std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> EntityTileMapCollisionStepper::get_entities() const
{
   return entities;
}


float EntityTileMapCollisionStepper::get_floor_height()
{
   return floor_height;
}


float EntityTileMapCollisionStepper::get_auto_ascend_threshold()
{
   return auto_ascend_threshold;
}


float EntityTileMapCollisionStepper::get_offset_at_collision_edge()
{
   return offset_at_collision_edge;
}


std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> EntityTileMapCollisionStepper::get_events_from_last_processed_step() const
{
   return events_from_last_processed_step;
}


void EntityTileMapCollisionStepper::process_step()
{
   if (!(tile_map))
   {
      std::stringstream error_message;
      error_message << "[EntityTileMapCollisionStepper::process_step]: error: guard \"tile_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("EntityTileMapCollisionStepper::process_step: error: guard \"tile_map\" not met");
   }
   events_from_last_processed_step.clear();
   float ceiling_height = tile_map->get_ceiling_height();

   for (auto &entity : entities)
   {
      AllegroFlare::vec2d player_xy = AllegroFlare::vec2d(
           entity->get_velocity_ref().position.x,
           entity->get_velocity_ref().position.y
        );

      AllegroFlare::vec2d x_y_movement_normalized = player_xy.normalized();
      float player_xy_magnitude = player_xy.get_magnitude();

      if (fabs(entity->get_velocity_ref().position.x) <= 0.00001 && fabs(entity->get_velocity_ref().position.y) <= 0.00001)
      {
         x_y_movement_normalized = AllegroFlare::vec2d(0, 1);
         player_xy_magnitude = 0.0;
      }

      double dirX = x_y_movement_normalized.x;
      double dirY = x_y_movement_normalized.y; // normal
      double dirZ = entity->get_velocity_ref().position.z;
      double moveSpeed = player_xy_magnitude;
      double posX = entity->get_placement_ref().position.x;
      double posY = entity->get_placement_ref().position.y;
      double posZ = entity->get_placement_ref().position.z;



      //
      // Evaluate tile collisions along the x axis
      //

      if(tile_map->get_tile(int(posX + dirX * moveSpeed), int(posY)).get_height() <= (posZ + get_auto_ascend_threshold()))
      {
         float previous_posX = posX;
         posX += dirX * moveSpeed;
         float now_posX = posX;

         // if currentx!=nextx, create a collision event into the nextx tile, with a face of NONE
         if ((int)previous_posX != (int)now_posX)
         {
           //AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity =
               //entity;
           //LabyrinthOfLore::Entity::Base* colliding_entity = entity;
           int collided_tile_x = int(posX);
           int collided_tile_y = int(posY);
           int collided_tile_type = tile_map->get_tile(collided_tile_x, collided_tile_y).get_type();
           LabyrinthOfLore::WorldMap::tile_face_t collided_tile_face_collided_with =
              LabyrinthOfLore::WorldMap::TILE_FACE_NONE;
           float collided_force = abs(dirX * moveSpeed);

            // a new tile space has been entered
            LabyrinthOfLore::Physics::EntityTileMapCollisionEvent collision_event(
                 entity, //colliding_entity,
                 collided_tile_type,
                 collided_tile_x,
                 collided_tile_y,
                 collided_tile_face_collided_with,
                 collided_force
               );
            events_from_last_processed_step.push_back(collision_event);
         }
      }
      else
      {
         AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity = entity;
         //LabyrinthOfLore::Entity::Base* colliding_entity = entity;
         int collided_tile_type = tile_map->get_tile(int(posX + dirX * moveSpeed), int(posY)).get_type();
         int collided_tile_x = int(posX + dirX * moveSpeed);
         int collided_tile_y = int(posY);
         LabyrinthOfLore::WorldMap::tile_face_t collided_tile_face_collided_with = ((dirX * moveSpeed) > 0) ? LabyrinthOfLore::WorldMap::TILE_FACE_LEFT : LabyrinthOfLore::WorldMap::TILE_FACE_RIGHT;
         float collided_force = abs(dirX * moveSpeed);

         LabyrinthOfLore::Physics::EntityTileMapCollisionEvent collision_event(
               entity, //colliding_entity,
               collided_tile_type,
               collided_tile_x,
               collided_tile_y,
               collided_tile_face_collided_with,
               collided_force
            );
         events_from_last_processed_step.push_back(collision_event);
      }



      //
      // Evaluate tile collisions along the y axis
      //

      if(tile_map->get_tile(int(posX), int(posY + dirY * moveSpeed)).get_height() <= (posZ + get_auto_ascend_threshold()))
      {
         float previous_posY = posY;
         posY += dirY * moveSpeed;
         float now_posY = posY;

         // if currenty!=nexty, create a collision event into the nexty tile, with a face of NONE
         if ((int)previous_posY != (int)now_posY)
         {
           //AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity = entity;
           //LabyrinthOfLore::Entity::Base* colliding_entity = entity;
           int collided_tile_x = int(posX);
           int collided_tile_y = int(posY);
           int collided_tile_type = tile_map->get_tile(collided_tile_x, collided_tile_y).get_type();
           LabyrinthOfLore::WorldMap::tile_face_t collided_tile_face_collided_with = LabyrinthOfLore::WorldMap::TILE_FACE_NONE;
           float collided_force = abs(dirY * moveSpeed);

            // a new tile space has been entered
            LabyrinthOfLore::Physics::EntityTileMapCollisionEvent collision_event(
                 entity, //colliding_entity,
                 collided_tile_type,
                 collided_tile_x,
                 collided_tile_y,
                 collided_tile_face_collided_with,
                 collided_force
               );
            events_from_last_processed_step.push_back(collision_event);
         }
      }
      else
      {
         //AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity = entity;
         //LabyrinthOfLore::Entity::Base* colliding_entity = entity;
         int collided_tile_x = int(posX);
         int collided_tile_y = int(posY + dirY * moveSpeed);
         int collided_tile_type = tile_map->get_tile(collided_tile_x, collided_tile_y).get_type();
         LabyrinthOfLore::WorldMap::tile_face_t collided_tile_face_collided_with = ((dirY * moveSpeed) > 0) ? LabyrinthOfLore::WorldMap::TILE_FACE_BACK : LabyrinthOfLore::WorldMap::TILE_FACE_FRONT;
         float collided_force = abs(dirY * moveSpeed);

         LabyrinthOfLore::Physics::EntityTileMapCollisionEvent collision_event(
               entity, //colliding_entity,
               collided_tile_type,
               collided_tile_x,
               collided_tile_y,
               collided_tile_face_collided_with,
               collided_force
            );

         events_from_last_processed_step.push_back(collision_event);
      }


      //
      // Evaluate tile collisions along the z axis
      //

      if ((posZ + dirZ) < tile_map->get_tile(int(posX), int(posY)).get_height())
      {
         posZ = tile_map->get_tile(int(posX), int(posY)).get_height() + get_offset_at_collision_edge();
         entity->get_velocity_ref().position.z = 0.0f;

         //AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* colliding_entity = entity;
         //LabyrinthOfLore::Entity::Base* colliding_entity = entity;
         int collided_tile_x = int(posX);
         int collided_tile_y = int(posY);
         int collided_tile_type = tile_map->get_tile(collided_tile_x, collided_tile_y).get_type();
         LabyrinthOfLore::WorldMap::tile_face_t collided_tile_face_collided_with = LabyrinthOfLore::WorldMap::TILE_FACE_TOP;
         float collided_force = abs(dirZ);

         LabyrinthOfLore::Physics::EntityTileMapCollisionEvent collision_event(
               entity, //colliding_entity,
               collided_tile_type,
               collided_tile_x,
               collided_tile_y,
               collided_tile_face_collided_with,
               collided_force
            );

         events_from_last_processed_step.push_back(collision_event);
      }
      else
      {
         posZ += dirZ;
      }


      //
      // Position the entity to its new location
      //

      entity->get_placement_ref().position.x = posX;
      entity->get_placement_ref().position.y = posY;
      //float clamped_ceiling = std::min<float>(get_ceiling_height()-get_offset_at_collision_edge(), posZ);


      //
      // Clamp the entity's z position to the z floor and ceiling
      //

      float clamped_ceiling = std::min<float>(ceiling_height-get_offset_at_collision_edge(), posZ);
      if (fabs(clamped_ceiling - posZ) > 0.0001f)
      {
         // has been clamped at the ceiling
         entity->get_velocity_ref().position.z = 0.0f;
      }

      float clamped_floor = std::max<float>(get_floor_height()+get_offset_at_collision_edge(), clamped_ceiling);
      if (fabs(clamped_floor - posZ) > 0.0001f)
      {
         // has been clamped at the ceiling
         entity->get_velocity_ref().position.z = 0.0f;
      }

      entity->get_placement_ref().position.z = clamped_floor;
   }

   return;
}


} // namespace Physics
} // namespace LabyrinthOfLore


