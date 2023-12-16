
#include <gtest/gtest.h>

#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/SceneRenderer2.hpp>
#include <AllegroFlare/Testing/WithAllegroRenderingFixture.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityFactory.hpp>
#include <AllegroFlare/Camera2D.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/SceneGraph/EntityPool.hpp>
#include <AllegroFlare/CubemapBuilder.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityRenderFlags.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/StaticMultitextureModel3D.hpp>
#include <AllegroFlare/Testing/TestNameInference.hpp>


class AllegroFlare_GraphicsPipelines_DynamicEntityPipeline_SceneRendererTest : public ::testing::Test {};
class AllegroFlare_GraphicsPipelines_DynamicEntityPipeline_SceneRendererTestWithAllegroRenderingFixtureTest :
   public AllegroFlare::Testing::WithAllegroRenderingFixture
{
public:
   virtual void SetUp() override
   {
      //set_display_width(1920*3/2);
      //set_display_height(1080*3/2);
      set_display_samples(0);
      AllegroFlare::Testing::WithAllegroRenderingFixture::SetUp();
   }
};


TEST_F(AllegroFlare_GraphicsPipelines_DynamicEntityPipeline_SceneRendererTest, can_be_created_without_blowing_up)
{
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::SceneRenderer2 scene_renderer;
}


TEST_F(AllegroFlare_GraphicsPipelines_DynamicEntityPipeline_SceneRendererTestWithAllegroRenderingFixtureTest,
   VISUAL__render__with_a_shadow_depth_map__will_render)
{
   AllegroFlare::ModelBin model_bin;
   model_bin.set_full_path(get_fixtures_path() + "models");

   // Create our entity pool
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool entity_pool;

   // Create our graphics pipeline
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::SceneRenderer2 scene_renderer;
   scene_renderer.set_entity_pool(&entity_pool);
   scene_renderer.setup_result_surface_bitmap(1920 / 3, 1080 / 3);
   scene_renderer.setup_shadow_map_buffer();
   scene_renderer.setup_cubemapping(get_fixtures_path() + "bitmaps/black_prism_1-01.png");
   scene_renderer.setup_multitexture_shader();

   // Create our entity factory
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityFactory entity_factory;
   entity_factory.set_bitmap_bin(&get_bitmap_bin_ref());
   entity_factory.set_model_bin(&model_bin);

   // Add objects to our scene
   // Create the camera
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Camera3D* camera_entity =
      entity_factory.create_camera_3d();
   AllegroFlare::Camera3D &camera = camera_entity->get_camera_3d_ref();
   camera_entity->set("primary_camera");
   camera.stepout = { 0, 1.0, 4.0 };
   camera.spin = 0.1;
   entity_pool.add(camera_entity);

   // TODO: Use an EntityFactory for this setup
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *item = 
      new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   item->set_model_3d(model_bin.auto_get("rounded_unit_cube-01.obj"));
   item->set(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityRenderFlags::RENDER_WITH_SKYBOX);
   item->get_placement_ref().position.y = 1.0;
   item->get_placement_ref().rotation.x = 0.05;
   item->get_placement_ref().rotation.z = 0.03547;
   entity_pool.add(item);

   // TODO: Use an EntityFactory for this setup
   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *dynamic_cube = 
      new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   dynamic_cube->set_model_3d(model_bin.auto_get("rounded_unit_cube-01.obj"));
   dynamic_cube->set_model_3d_texture(get_bitmap_bin_ref().auto_get("uv.png"));
   dynamic_cube->get_placement_ref().position.x = 1.5;
   dynamic_cube->get_placement_ref().position.y = 0.5;
   entity_pool.add(dynamic_cube);

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D *env = 
      new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D();
   env->set_model_3d(model_bin.auto_get("simple_scene-01.obj"));
   //rounded_unit_cube-01.obj"));
   env->set_model_3d_texture(get_bitmap_bin_ref().auto_get("simple_scene-01-1024.jpg"));
   env->get_placement_ref().position.x = 0;
   env->get_placement_ref().position.y = 0;
   entity_pool.add(env);

   /*
   // TODO: Use an EntityFactory for this setup
   std::string TEST_FIXTURES_FOLDER = get_fixtures_path();
   std::string base_obj_filename = TEST_FIXTURES_FOLDER + "models/simple_scene-01.obj";
   std::string uv2_obj_filename = TEST_FIXTURES_FOLDER + "models/simple_scene-01-ao-01.obj";
   AllegroFlare::MultitextureModel3D multitexture_model;
   multitexture_model.initialize();
   multitexture_model.load_obj_file(base_obj_filename, uv2_obj_filename.c_str());

   AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::StaticMultitextureModel3D *environment_mesh = 
      new AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::StaticMultitextureModel3D();
   environment_mesh->set_multitexture_model_3d(&multitexture_model);
   environment_mesh->set_multitexture_model_3d_texture_1(get_bitmap_bin_ref().auto_get("simple_scene-01-1024.jpg"));
   environment_mesh->set_multitexture_model_3d_texture_2(get_bitmap_bin_ref().auto_get("simple_scene-01-ao-01.jpg"));
   entity_pool.add(environment_mesh);
   */

   // Render the scene
   int frames = 90 * 5;
   for (int i=0; i<frames; i++)
   {
      //AllegroFlare::Camera3D *light = scene_renderer.get_shadow_map_buffer_ref().get_light();
      // Spin our shadow casted light
      //light->spin -= 0.001f;

      // Rotate objects in the scene
      //item->get_placement_ref().rotation.x += 0.005;
      //item->get_placement_ref().rotation.z += 0.003547;

      // Pan the camera
      //camera.stepout.z += 0.01;
      //camera.spin += 0.0005;
      //camera.tilt += 0.0008;

      // Render the scene
      scene_renderer.render();

      // Show the depth pass
      AllegroFlare::Camera2D camera2d;
      ALLEGRO_BITMAP *backbuffer = al_get_backbuffer(al_get_current_display());

      al_set_target_bitmap(backbuffer);
      //camera2d.setup_dimensional_projection(backbuffer);
      //ALLEGRO_BITMAP *render_surface = al_get_backbuffer(al_get_current_display()); // TODO: replace with render surface
      //al_identity_transform(&render_surface);

      al_clear_depth_buffer(1);
      al_clear_to_color(ALLEGRO_COLOR{0.1, 0.105, 0.12, 1.0});

      ALLEGRO_BITMAP *render_surface = scene_renderer.get_render_surface_ref().obtain_surface();
      al_draw_scaled_bitmap(
         //shadow_depth_map_renderer.get_result_surface_bitmap(),
         //depth_pass.get_result_surface_bitmap(),
         render_surface,
         0,
         0,
         al_get_bitmap_width(render_surface),
         al_get_bitmap_height(render_surface),
         0,
         0,
         al_get_display_width(get_display()),
         al_get_display_height(get_display()),
         //ALLEGRO_COLOR{1.0, 1.0, 1.0, 1.0},
         //ALLEGRO_COLOR{0.4, 0.4, 0.4, 0.4},
         //0,
         //0,
         0
      );

      al_flip_display();
      al_rest(1.0/60.0f);
   }

   std::string snapshot_filename = AllegroFlare::Testing::TestNameInference::build_test_snapshot_full_filename();
   ALLEGRO_BITMAP *rendered_shadow_depth_map_result = scene_renderer.get_render_surface_ref().obtain_surface();
   al_save_bitmap("/Users/markoates/Desktop/foobar.png", rendered_shadow_depth_map_result);
}


