
#include <gtest/gtest.h>

#include <AllegroFlare/Shaders/ShadowMapping.hpp>


TEST(AllegroFlare_Shaders_ShadowMappingTest, can_be_created_without_blowing_up)
{
   AllegroFlare::Shaders::ShadowMapping shadow_mapping;
}


TEST(AllegroFlare_Shaders_ShadowMappingTest, initialize__will_not_blow_up)
{
   // TODO: Use a relative test fixture path
   //std::string fixture_data_path_for_shaders = "/Users/markoates/Repos/Pipeline/tests/fixtures/shaders/";
   std::string fixture_data_path_for_shaders = "/Users/markoates/Repos/Pipeline/bin/data/shaders/";

   al_init();
   al_set_new_display_flags(ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL);
   ALLEGRO_DISPLAY *display = al_create_display(1920, 1080);
   AllegroFlare::Shaders::ShadowMapping shadow_mapping;
   shadow_mapping.set_data_path_for_shaders(fixture_data_path_for_shaders);
   shadow_mapping.initialize();
   al_destroy_display(display);
   al_uninstall_system();
}


//TEST(AllegroFlare_Shaders_ShadowMappingTest, can_be_created_without_blowing_up)
//{
   //AllegroFlare::Shaders::ShadowMapping shadow_mapping;
//}


/*
TEST(AllegroFlare_Shaders_AllegroDefaultTest, TYPE__has_the_expected_value)
{
   AllegroFlare::Shaders::AllegroDefault shader;
   EXPECT_EQ("AllegroFlare/Shaders/AllegroDefault", shader.get_type());
}


TEST(AllegroFlare_Shaders_AllegroDefaultTest, type__has_the_expected_value_matching_TYPE)
{
   AllegroFlare::Shaders::AllegroDefault shader;
   EXPECT_EQ(AllegroFlare::Shaders::AllegroDefault::TYPE, shader.get_type());
}
*/


/* // TODO: add in this test environment
TEST_F(AllegroFlare_Shaders_CubeMapWithAllegroRenderingFixtureTest, VISUAL__will_appear_as_expected)
{
   AllegroFlare::Shaders::CubeMap shader;
   AllegroFlare::Camera3D camera;
   AllegroFlare::ModelBin model_bin;
   AllegroFlare::Model3D *model;

   model_bin.set_path(get_fixtures_path() + "models");
   model = model_bin["rounded_unit_cube-01.obj"];

   // TODO: setup object

   shader.initialize();
   shader.activate();

   int passes = 60;
   for (int i=0; i<passes; i++)
   {
      camera.setup_projection_on(get_display_backbuffer());
      clear();

      model->draw();

      // TODO: render object
      al_flip_display();
   }

   shader.deactivate();
}
*/


