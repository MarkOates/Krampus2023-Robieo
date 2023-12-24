
#include <gtest/gtest.h>

#include <Pipeline/Physics/GravityStepper.hpp>


TEST(Pipeline_Physics_GravityStepperTest, can_be_created_without_blowing_up)
{
   Pipeline::Physics::GravityStepper gravity_stepper;
}


