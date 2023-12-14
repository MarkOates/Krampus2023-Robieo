
#include <gtest/gtest.h>

#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/ShadowDepthMapRenderer2.hpp>


TEST(AllegroFlare_GraphicsPipelines_DynamicEntityPipeline_ShadowDepthMapRenderer2Test, can_be_created_without_blowing_up)
{
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2 shadow_depth_map_renderer2;
}


TEST(AllegroFlare_GraphicsPipelines_DynamicEntityPipeline_ShadowDepthMapRenderer2Test, run__returns_the_expected_response)
{
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2 shadow_depth_map_renderer2;
   std::string expected_string = "Hello World!";
   EXPECT_EQ(expected_string, shadow_depth_map_renderer2.run());
}


