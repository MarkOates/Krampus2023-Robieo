  - symbol: AllegroFlare::Physics::AABB3D
    headers: [ AllegroFlare/Physics/AABB3D.hpp ]

#include <gtest/gtest.h>

#include <Pipeline/Gameplay/LevelCameraZone.hpp>


TEST(Pipeline_Gameplay_LevelCameraZoneTest, can_be_created_without_blowing_up)
{
   Pipeline::Gameplay::LevelCameraZone level_camera_zone;
}


