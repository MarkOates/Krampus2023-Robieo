

#include <LabyrinthOfLore/Physics/EntityTileMapCollisionEvent.hpp>




namespace LabyrinthOfLore
{
namespace Physics
{


EntityTileMapCollisionEvent::EntityTileMapCollisionEvent(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* entity, int tile_type, int tile_x, int tile_y, LabyrinthOfLore::WorldMap::tile_face_t tile_face_collided_with, float force)
   : entity(entity)
   , tile_type(tile_type)
   , tile_x(tile_x)
   , tile_y(tile_y)
   , tile_face_collided_with(tile_face_collided_with)
   , force(force)
{
}


EntityTileMapCollisionEvent::~EntityTileMapCollisionEvent()
{
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D* EntityTileMapCollisionEvent::get_entity() const
{
   return entity;
}


int EntityTileMapCollisionEvent::get_tile_type() const
{
   return tile_type;
}


int EntityTileMapCollisionEvent::get_tile_x() const
{
   return tile_x;
}


int EntityTileMapCollisionEvent::get_tile_y() const
{
   return tile_y;
}


LabyrinthOfLore::WorldMap::tile_face_t EntityTileMapCollisionEvent::get_tile_face_collided_with() const
{
   return tile_face_collided_with;
}


float EntityTileMapCollisionEvent::get_force() const
{
   return force;
}




} // namespace Physics
} // namespace LabyrinthOfLore


