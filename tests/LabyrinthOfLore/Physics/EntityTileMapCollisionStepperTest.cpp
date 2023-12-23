
#include <gtest/gtest.h>

#include <LabyrinthOfLore/Physics/EntityTileMapCollisionStepper.hpp>

#define FLOATING_POINT_ERROR_MARGIN (0.00001f)


static void EXPECT_EQ_COLLISION_EVENT(LabyrinthOfLore::Physics::EntityTileMapCollisionEvent &expected, LabyrinthOfLore::Physics::EntityTileMapCollisionEvent &actual)
{
   EXPECT_EQ(expected.get_entity(), actual.get_entity());
   EXPECT_EQ(expected.get_tile_type(), actual.get_tile_type());
   EXPECT_EQ(expected.get_tile_x(), actual.get_tile_x());
   EXPECT_EQ(expected.get_tile_y(), actual.get_tile_y());
   EXPECT_EQ(expected.get_tile_face_collided_with(), actual.get_tile_face_collided_with());
   EXPECT_EQ(expected.get_force(), actual.get_force());
}


TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, can_be_created_without_blowing_up)
{
   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper;
}


TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, accepts_reasonable_arguments_without_crashing)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
    std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = {};

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   SUCCEED();
}


TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, process_step__works_with_reasonable_arguments)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = {};

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);

   entity_tile_map_collision_stepper.process_step();
   SUCCEED();
}


TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__steps_the_entity_forward_by_its_velocity)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(10, 10, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.0, 0.0, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.3, 0.4, 0.2);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_EQ(0.3f, actual_placement.position.x);
   EXPECT_EQ(0.4f, actual_placement.position.y);
   EXPECT_NEAR(10.201f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   AllegroFlare::Placement3D actual_velocity = entity.get_velocity_ref();
   EXPECT_EQ(0.3f, actual_velocity.position.x);
   EXPECT_EQ(0.4f, actual_velocity.position.y);
   EXPECT_EQ(0.2f, actual_velocity.position.z);
}


TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__steps_the_entity_forward_when_moving_at_a_negative_velocity)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(10, 10, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity =
      AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities =
      { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(1.0, 1.0, 11.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(-0.3, -0.4, -0.2);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_EQ(0.7f, actual_placement.position.x);
   EXPECT_EQ(0.6f, actual_placement.position.y);
   EXPECT_NEAR(10.801f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   AllegroFlare::Placement3D actual_velocity = entity.get_velocity_ref();
   EXPECT_EQ(-0.3f, actual_velocity.position.x);
   EXPECT_EQ(-0.4f, actual_velocity.position.y);
   EXPECT_EQ(-0.2f, actual_velocity.position.z);
}


TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__does_not_horizontally_advance_the_player_if_the_colliding_block_is_higher_than_the_auto_ascend_threshold)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   float auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(1.0, 0.0, 0.0);
   tile_map.set_tile(1, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.001 + auto_ascend_threshold + 0.001));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_EQ(0.5f, actual_placement.position.x);
   EXPECT_EQ(0.5f, actual_placement.position.y);
   EXPECT_NEAR(10.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   AllegroFlare::Placement3D actual_velocity = entity.get_velocity_ref();
   EXPECT_EQ(1.0f, actual_velocity.position.x);
   EXPECT_EQ(0.0f, actual_velocity.position.y);
   EXPECT_EQ(0.0f, actual_velocity.position.z);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__does_not_vertically_advance_the_player_if_the_colliding_block_is_higher_than_the_auto_ascend_threshold)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(1, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   float auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, 1.0, 0.0);
   tile_map.set_tile(0, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.001 + auto_ascend_threshold + 0.001));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_EQ(0.5f, actual_placement.position.x);
   EXPECT_EQ(0.5f, actual_placement.position.y);
   EXPECT_NEAR(10.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   AllegroFlare::Placement3D actual_velocity = entity.get_velocity_ref();
   EXPECT_EQ(0.0f, actual_velocity.position.x);
   EXPECT_EQ(1.0f, actual_velocity.position.y);
   EXPECT_EQ(0.0f, actual_velocity.position.z);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      has_an_auto_ascend_threshold_that_is_one_quarter_of_a_whole_unit)
{
   float expected_auto_ascend_threshold = 0.25;
   float actual_auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();

   ASSERT_EQ(expected_auto_ascend_threshold, actual_auto_ascend_threshold);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__if_falling_beyond_a_block_height__will_position_the_entity_slightly_above_the_height_of_the_tile__and_will_set_the_entitys_vertical_velocity_to_zero)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(1, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.5);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, 0.0, -1.0);
   tile_map.set_tile(0, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_EQ(0.5f, actual_placement.position.x);
   EXPECT_EQ(0.5f, actual_placement.position.y);
   EXPECT_NEAR(10.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   AllegroFlare::Placement3D actual_velocity = entity.get_velocity_ref();
   EXPECT_EQ(0.0f, actual_velocity.position.x);
   EXPECT_EQ(0.0f, actual_velocity.position.y);
   EXPECT_EQ(0.0f, actual_velocity.position.z);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__does_advance_the_player_to_a_higher_block_if_the_colliding_block_is_not_higher_than_the_auto_ascent_threshold__and_repositions_the_entity_slightly_above_the_height_of_the_tile__and_sets_the_player_vertical_velocity_to_zero)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   float auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(1.0, 0.0, -4.0);
   tile_map.set_tile(1, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_EQ(1.5f, actual_placement.position.x);
   EXPECT_EQ(0.5f, actual_placement.position.y);
   EXPECT_NEAR(10.0f + auto_ascend_threshold + 0.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   AllegroFlare::Placement3D actual_velocity = entity.get_velocity_ref();
   EXPECT_EQ(1.0f, actual_velocity.position.x);
   EXPECT_EQ(0.0f, actual_velocity.position.y);
   EXPECT_EQ(0.0f, actual_velocity.position.z);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, process_step__processes_multiple_entities)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(3, 3, LabyrinthOfLore::WorldMap::Tile(0, 10.0));

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity1 = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity2 = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity3 = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();

   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity1, &entity2, &entity3 };

   entity1.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);
   entity1.get_velocity_ref().position = AllegroFlare::vec3d(1.0, 0.0, 0.0);

   entity2.get_placement_ref().position = AllegroFlare::vec3d(0.5, 1.5, 10.001);
   entity2.get_velocity_ref().position = AllegroFlare::vec3d(0.0, -1.0, 0.0);

   entity3.get_placement_ref().position = AllegroFlare::vec3d(1.5, 0.5, 10.001);
   entity3.get_velocity_ref().position = AllegroFlare::vec3d(-1.0, 0.0, 0.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D entity1_actual_placement = entity1.get_placement_ref();
   AllegroFlare::Placement3D entity2_actual_placement = entity2.get_placement_ref();
   AllegroFlare::Placement3D entity3_actual_placement = entity3.get_placement_ref();

   EXPECT_EQ(1.5f, entity1_actual_placement.position.x);
   EXPECT_EQ(0.5f, entity1_actual_placement.position.y);
   EXPECT_NEAR(10.001f, entity1_actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   EXPECT_EQ(0.5f, entity2_actual_placement.position.x);
   EXPECT_EQ(0.5f, entity2_actual_placement.position.y);
   EXPECT_NEAR(10.001f, entity2_actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   EXPECT_EQ(0.5f, entity3_actual_placement.position.x);
   EXPECT_EQ(0.5f, entity3_actual_placement.position.y);
   EXPECT_NEAR(10.001f, entity3_actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__will_not_move_an_entity_higher_than_the_ceiling__but_will_position_it_at_the_maximum_minus_a_small_offset__and_will_set_the_vertical_velocity_to_0)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(10, 10, LabyrinthOfLore::WorldMap::Tile(0, -10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.0, 0.0, 99.501);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, 0.0, 1.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_EQ(0.0f, actual_placement.position.x);
   EXPECT_EQ(0.0f, actual_placement.position.y);
   EXPECT_EQ(99.999f, actual_placement.position.z);

   AllegroFlare::Placement3D actual_velocity = entity.get_velocity_ref();
   EXPECT_EQ(0.0f, actual_velocity.position.x);
   EXPECT_EQ(0.0f, actual_velocity.position.y);
   EXPECT_EQ(0.0f, actual_velocity.position.z);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__uses_the_ceiling_height_from_the_tile_map)
{
   float ceiling_height = 4.0f;
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(10, 10, LabyrinthOfLore::WorldMap::Tile(0, -10.0));
   tile_map.set_ceiling_height(ceiling_height);
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.0, 0.0, ceiling_height - 0.5);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, 0.0, 1.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_EQ(0.0f, actual_placement.position.x);
   EXPECT_EQ(0.0f, actual_placement.position.y);
   EXPECT_EQ(3.999f, actual_placement.position.z);

   AllegroFlare::Placement3D actual_velocity = entity.get_velocity_ref();
   EXPECT_EQ(0.0f, actual_velocity.position.x);
   EXPECT_EQ(0.0f, actual_velocity.position.y);
   EXPECT_EQ(0.0f, actual_velocity.position.z);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__will_not_move_an_entity_lower_than_the_floor__but_will_position_it_at_the_miniumum_plus_a_small_offset__and_will_set_the_vertical_velocity_to_0)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(10, 10, LabyrinthOfLore::WorldMap::Tile(0, -10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.0, 0.0, 0.501);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, 0.0, -1.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_EQ(0.0f, actual_placement.position.x);
   EXPECT_EQ(0.0f, actual_placement.position.y);
   EXPECT_EQ(0.001f, actual_placement.position.z);

   AllegroFlare::Placement3D actual_velocity = entity.get_velocity_ref();
   EXPECT_EQ(0.0f, actual_velocity.position.x);
   EXPECT_EQ(0.0f, actual_velocity.position.y);
   EXPECT_EQ(0.0f, actual_velocity.position.z);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_setp__does_not_have_range_restrictions_on_the_minimum_edges_of_the_map)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(1, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(-1.0, -1.0, 0.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_NEAR(-0.5f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(-0.5f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(10.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_setp__does_not_have_range_restrictions_on_the_maximum_edges_of_the_map)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(1, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(1.0, 1.0, 0.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_NEAR(1.5f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(1.5f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(10.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      process_setp__multiple_pass_1__ascending_stairs_horizontally)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(9, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   float auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();
   tile_map.set_tile(0, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*0));
   tile_map.set_tile(1, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*1));
   tile_map.set_tile(2, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*2));
   tile_map.set_tile(3, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*3));
   tile_map.set_tile(4, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*4));
   tile_map.set_tile(5, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*5));
   tile_map.set_tile(6, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*6));
   tile_map.set_tile(7, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*7));
   tile_map.set_tile(8, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*8));

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(1.0, 0.0, 0.0);

   for (unsigned pass=0; pass<8; pass++)
   {
      LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
      entity_tile_map_collision_stepper.process_step();
   }

   AllegroFlare::Placement3D actual_placement = entity.get_placement_ref();
   EXPECT_NEAR(8.5f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(0.5f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(12.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      process_step__multiple_pass_2__spiraling_staricase)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(4, 4, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   float auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();
   AllegroFlare::Placement3D &actual_placement = entity.get_placement_ref();

   // moving right
   tile_map.set_tile(0, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*0));
   tile_map.set_tile(1, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*1));
   tile_map.set_tile(2, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*2));
   tile_map.set_tile(3, 0, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*3));

   // moving down
   tile_map.set_tile(3, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*4));
   tile_map.set_tile(3, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*5));
   tile_map.set_tile(3, 3, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*6));

   // moving left
   tile_map.set_tile(2, 3, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*7));
   tile_map.set_tile(1, 3, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*8));
   tile_map.set_tile(0, 3, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*9));

   // moving up
   tile_map.set_tile(0, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*10));
   tile_map.set_tile(0, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0 + auto_ascend_threshold*11));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);

   // moving right
   entity.get_velocity_ref().position = AllegroFlare::vec3d(1.0, 0.0, 0.0);

   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();

   EXPECT_NEAR(3.5f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(0.5f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(10.75f + 0.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   // moving down
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, 1.0, 0.0);

   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();

   EXPECT_NEAR(3.5f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(3.5f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(11.5f + 0.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   // moving left
   entity.get_velocity_ref().position = AllegroFlare::vec3d(-1.0, 0.0, 0.0);

   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();

   EXPECT_NEAR(0.5f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(3.5f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(12.25f + 0.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);

   // moving up
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, -1.0, 0.0);

   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();

   EXPECT_NEAR(0.5f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(1.5f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(12.75f + 0.001f, actual_placement.position.z, FLOATING_POINT_ERROR_MARGIN);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      process_step__diagonal_slide_against_wall__down_right__has_an_x_first_bias)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 3, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   tile_map.set_tile(1, 0, LabyrinthOfLore::WorldMap::Tile(0, 11.0));

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.75, 0.25, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.5, 0.5, 0.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);

   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D &actual_placement = entity.get_placement_ref();
   EXPECT_NEAR(1.25f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(1.75f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      process_step__diagonal_slide_against_wall__down_left__has_an_x_first_bias)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   tile_map.set_tile(0, 0, LabyrinthOfLore::WorldMap::Tile(0, 11.0));

   entity.get_placement_ref().position = AllegroFlare::vec3d(1.25, 0.25, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(-0.5, 0.5, 0.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);

   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D &actual_placement = entity.get_placement_ref();
   EXPECT_NEAR(0.75f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(1.75f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      process_step__diagonal_slide_against_wall__up_right__has_an_x_first_bias)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   tile_map.set_tile(1, 1, LabyrinthOfLore::WorldMap::Tile(0, 11.0));

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.75, 1.75, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.5, -0.5, 0.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);

   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D &actual_placement = entity.get_placement_ref();
   EXPECT_NEAR(1.25f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(0.25f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      process_step__diagonal_slide_against_wall__up_left__has_an_x_first_bias)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   tile_map.set_tile(0, 1, LabyrinthOfLore::WorldMap::Tile(0, 11.0));

   entity.get_placement_ref().position = AllegroFlare::vec3d(1.25, 1.75, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(-0.5, -0.5, 0.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);

   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();
   entity_tile_map_collision_stepper.process_step();

   AllegroFlare::Placement3D &actual_placement = entity.get_placement_ref();
   EXPECT_NEAR(0.75f, actual_placement.position.x, FLOATING_POINT_ERROR_MARGIN);
   EXPECT_NEAR(0.25f, actual_placement.position.y, FLOATING_POINT_ERROR_MARGIN);
}

// collision event responses

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, events_from_last_processed_step__has_a_getter_and_has_the_expected_default_value)
{
   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper;

   entity_tile_map_collision_stepper.get_events_from_last_processed_step();
   SUCCEED();
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, get_events_from_last_processed_step__after_a_step__returns_the_expected_collision_events__moving_right)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   float auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(1.0, 0.0, 0.0);
   tile_map.set_tile(1, 0, LabyrinthOfLore::WorldMap::Tile(13, 10.001 + auto_ascend_threshold + 0.001));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> expected_collision_events = {
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 13, 1, 0, LabyrinthOfLore::WorldMap::TILE_FACE_LEFT, 1.0)
   };

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> actual_collision_events = entity_tile_map_collision_stepper.get_events_from_last_processed_step();

   ASSERT_EQ(expected_collision_events.size(), actual_collision_events.size());

   for (unsigned i=0; i<expected_collision_events.size(); i++)
   {
      EXPECT_EQ_COLLISION_EVENT(expected_collision_events[i], actual_collision_events[i]);
   }
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, get_events_from_last_processed_step__after_a_step__returns_the_expected_collision_events__moving_left)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 1, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   float auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();

   entity.get_placement_ref().position = AllegroFlare::vec3d(1.5, 0.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(-0.971, 0.0, 0.0);
   tile_map.set_tile(0, 0, LabyrinthOfLore::WorldMap::Tile(7, 10.001 + auto_ascend_threshold + 0.001));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> expected_collision_events = {
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 7, 0, 0, LabyrinthOfLore::WorldMap::TILE_FACE_RIGHT, 0.971)
   };

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> actual_collision_events = entity_tile_map_collision_stepper.get_events_from_last_processed_step();

   ASSERT_EQ(expected_collision_events.size(), actual_collision_events.size());

   for (unsigned i=0; i<expected_collision_events.size(); i++)
   {
      EXPECT_EQ_COLLISION_EVENT(expected_collision_events[i], actual_collision_events[i]);
   }
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, get_events_from_last_processed_step__after_a_step__returns_the_expected_collision_events__moving_down)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(1, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   float auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 0.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, 0.897, 0.0);
   tile_map.set_tile(0, 1, LabyrinthOfLore::WorldMap::Tile(97, 10.001 + auto_ascend_threshold + 0.001));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> expected_collision_events = {
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 97, 0, 1, LabyrinthOfLore::WorldMap::TILE_FACE_BACK, 0.897)
   };

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> actual_collision_events = entity_tile_map_collision_stepper.get_events_from_last_processed_step();

   ASSERT_EQ(expected_collision_events.size(), actual_collision_events.size());

   for (unsigned i=0; i<expected_collision_events.size(); i++)
   {
      EXPECT_EQ_COLLISION_EVENT(expected_collision_events[i], actual_collision_events[i]);
   }
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, get_events_from_last_processed_step__after_a_step__returns_the_expected_collision_events__moving_up)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(1, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   float auto_ascend_threshold = LabyrinthOfLore::Physics::EntityTileMapCollisionStepper::get_auto_ascend_threshold();

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.5, 1.5, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, -0.918, 0.0);
   tile_map.set_tile(0, 0, LabyrinthOfLore::WorldMap::Tile(32, 10.001 + auto_ascend_threshold + 0.001));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> expected_collision_events = {
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 32, 0, 0, LabyrinthOfLore::WorldMap::TILE_FACE_FRONT, 0.918)
   };

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> actual_collision_events = entity_tile_map_collision_stepper.get_events_from_last_processed_step();

   ASSERT_EQ(expected_collision_events.size(), actual_collision_events.size());

   for (unsigned i=0; i<expected_collision_events.size(); i++)
   {
      EXPECT_EQ_COLLISION_EVENT(expected_collision_events[i], actual_collision_events[i]);
   }
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, get_events_from_last_processed_step__after_a_step__returns_the_expected_collision_events__when_falling)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(3, 3, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(2.5, 1.5, 10.5);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.0, 0.0, -0.729);
   tile_map.set_tile(2, 1, LabyrinthOfLore::WorldMap::Tile(129, 10.0));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> expected_collision_events = {
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 129, 2, 1, LabyrinthOfLore::WorldMap::TILE_FACE_TOP, 0.729)
   };

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> actual_collision_events = entity_tile_map_collision_stepper.get_events_from_last_processed_step();

   ASSERT_EQ(expected_collision_events.size(), actual_collision_events.size());

   for (unsigned i=0; i<expected_collision_events.size(); i++)
   {
      EXPECT_EQ_COLLISION_EVENT(expected_collision_events[i], actual_collision_events[i]);
   }
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, get_events_from_last_processed_step__after_a_step__returns_multiple_events_when_appropriate)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(3, 3, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   entity.get_placement_ref().position = AllegroFlare::vec3d(1.5, 1.5, 10.5);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.681, 0.829, -0.729);
   tile_map.set_tile(2, 1, LabyrinthOfLore::WorldMap::Tile(41, 20.0));
   tile_map.set_tile(1, 2, LabyrinthOfLore::WorldMap::Tile(96, 20.0));
   tile_map.set_tile(1, 1, LabyrinthOfLore::WorldMap::Tile(7, 10.0));

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> expected_collision_events = {
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 41, 2, 1, LabyrinthOfLore::WorldMap::TILE_FACE_LEFT, 0.681),
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 96, 1, 2, LabyrinthOfLore::WorldMap::TILE_FACE_BACK, 0.829),
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 7, 1, 1, LabyrinthOfLore::WorldMap::TILE_FACE_TOP, 0.729)
   };

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> actual_collision_events = entity_tile_map_collision_stepper.get_events_from_last_processed_step();

   ASSERT_EQ(expected_collision_events.size(), actual_collision_events.size());

   for (unsigned i=0; i<expected_collision_events.size(); i++)
   {
      EXPECT_EQ_COLLISION_EVENT(expected_collision_events[i], actual_collision_events[i]);
   }
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest, get_events_from_last_processed_step__after_a_step__returns_events_when_multiple_entities_are_involved)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0));

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity1 = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity2 = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity1, &entity2 };

   tile_map.set_tile(0, 0, LabyrinthOfLore::WorldMap::Tile(82, 20.0));
   tile_map.set_tile(1, 1, LabyrinthOfLore::WorldMap::Tile(928, 20.0));

   entity1.get_placement_ref().position = AllegroFlare::vec3d(0.5, 1.5, 10.5);
   entity1.get_velocity_ref().position = AllegroFlare::vec3d(0.0, -0.611, 0.0);

   entity2.get_placement_ref().position = AllegroFlare::vec3d(1.5, 0.5, 10.5);
   entity2.get_velocity_ref().position = AllegroFlare::vec3d(0.0, 0.987, 0.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> expected_collision_events = {
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity1, 82, 0, 0, LabyrinthOfLore::WorldMap::TILE_FACE_FRONT, 0.611),
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity2, 928, 1, 1, LabyrinthOfLore::WorldMap::TILE_FACE_BACK, 0.987),
   };

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> actual_collision_events = entity_tile_map_collision_stepper.get_events_from_last_processed_step();

   ASSERT_EQ(expected_collision_events.size(), actual_collision_events.size());

   for (unsigned i=0; i<expected_collision_events.size(); i++)
   {
      EXPECT_EQ_COLLISION_EVENT(expected_collision_events[i], actual_collision_events[i]);
   }
}

TEST(LabyrinthOfLore_Physics_EntityTileMapCollisionStepperTest,
      with_an_entity__process_step__creates_the_expected_collision_events_when_there_are_no_obstacles__multiple)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(2, 2, LabyrinthOfLore::WorldMap::Tile(0, 10.0));
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D entity = AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   std::vector<AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D*> entities = { &entity };

   tile_map.set_tile(1, 0, LabyrinthOfLore::WorldMap::Tile(82, 10.0));

   entity.get_placement_ref().position = AllegroFlare::vec3d(0.8, 0.9, 10.001);
   entity.get_velocity_ref().position = AllegroFlare::vec3d(0.3, 0.4, -2.0);

   LabyrinthOfLore::Physics::EntityTileMapCollisionStepper entity_tile_map_collision_stepper(tile_map, entities);
   entity_tile_map_collision_stepper.process_step();

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> expected_collision_events = {
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 82, 1, 0, LabyrinthOfLore::WorldMap::TILE_FACE_NONE, 0.3),
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 0, 1, 1, LabyrinthOfLore::WorldMap::TILE_FACE_NONE, 0.4),
      LabyrinthOfLore::Physics::EntityTileMapCollisionEvent(&entity, 0, 1, 1, LabyrinthOfLore::WorldMap::TILE_FACE_TOP, 2.0),
   };

   std::vector<LabyrinthOfLore::Physics::EntityTileMapCollisionEvent> actual_collision_events = entity_tile_map_collision_stepper.get_events_from_last_processed_step();

   ASSERT_EQ(expected_collision_events.size(), actual_collision_events.size());

   for (unsigned i=0; i<expected_collision_events.size(); i++)
   {
      EXPECT_EQ_COLLISION_EVENT(expected_collision_events[i], actual_collision_events[i]);
   }
}

