
#include <gtest/gtest.h>

#include <LabyrinthOfLore/Physics/EntityTileMapCollisionEvent.hpp>

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionEventTest, can_be_created_without_blowing_up)
{
   LabyrinthOfLore::Physics::EntityTileMapCollisionEvent entity_tile_map_collision_event;
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionEventTest, has_getters_for_all_properties_and_sets_expected_defaults)
{
   LabyrinthOfLore::Physics::EntityTileMapCollisionEvent entity_tile_map_collision_event;
   ASSERT_EQ(nullptr, entity_tile_map_collision_event.get_entity());
   ASSERT_EQ(0, entity_tile_map_collision_event.get_tile_type());
   ASSERT_EQ(0, entity_tile_map_collision_event.get_tile_x());
   ASSERT_EQ(0, entity_tile_map_collision_event.get_tile_y());
   ASSERT_EQ(LabyrinthOfLore::WorldMap::TILE_FACE_NONE, entity_tile_map_collision_event.get_tile_face_collided_with());
   ASSERT_EQ(0, entity_tile_map_collision_event.get_force());
}

