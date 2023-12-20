

#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/SceneRenderer2.hpp>

#include <AllegroFlare/Camera2D.hpp>
#include <AllegroFlare/CubemapBuilder.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/Camera3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/StaticModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/StaticMultitextureModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityRenderFlags.hpp>
#include <AllegroFlare/Logger.hpp>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace AllegroFlare
{
namespace GraphicsPipelines
{
namespace DynamicEntityPipeline
{


SceneRenderer2::SceneRenderer2(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool)
   : cubemap_shader()
   , multitexture_shader()
   , entity_pool(entity_pool)
   , data_path_for_shaders("")
   , shadow_map_buffer()
   , render_surface()
   , render_surface_is_setup(false)
   , cubemapping_is_setup(false)
   , multitexture_shader_is_setup(false)
   , shadow_map_buffer_is_setup(false)
{
}


SceneRenderer2::~SceneRenderer2()
{
}


void SceneRenderer2::set_entity_pool(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool)
{
   this->entity_pool = entity_pool;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* SceneRenderer2::get_entity_pool() const
{
   return entity_pool;
}


std::string SceneRenderer2::get_data_path_for_shaders() const
{
   return data_path_for_shaders;
}


AllegroFlare::Shaders::Cubemap &SceneRenderer2::get_cubemap_shader_ref()
{
   return cubemap_shader;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowMapBuffer &SceneRenderer2::get_shadow_map_buffer_ref()
{
   return shadow_map_buffer;
}


AllegroFlare::RenderSurfaces::Bitmap &SceneRenderer2::get_render_surface_ref()
{
   return render_surface;
}


void SceneRenderer2::set_data_path_for_shaders(std::string data_path_for_shaders)
{
   if (!((!shadow_map_buffer_is_setup)))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::set_data_path_for_shaders]: error: guard \"(!shadow_map_buffer_is_setup)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::set_data_path_for_shaders: error: guard \"(!shadow_map_buffer_is_setup)\" not met");
   }
   this->data_path_for_shaders = data_path_for_shaders;
   return;
}

void SceneRenderer2::setup_result_surface_bitmap(int width, int height)
{
   render_surface.set_surface_width(width);
   render_surface.set_surface_height(height);
   render_surface.set_multisamples(0);
   render_surface.set_depth(32);
   render_surface.initialize();

   render_surface_is_setup = true;

   return;
}

void SceneRenderer2::setup_shadow_map_buffer()
{
   shadow_map_buffer.set_entity_pool(entity_pool);
   shadow_map_buffer.set_data_path_for_shaders(data_path_for_shaders); //get_fixtures_path() + "shaders/");
   shadow_map_buffer.set_result_surface_width(1920 / 2);
   shadow_map_buffer.set_result_surface_height(1080 / 2);
   shadow_map_buffer.initialize();

   shadow_map_buffer_is_setup = true;
   return;
}

void SceneRenderer2::setup_cubemapping(std::string cube_map_texture_filename)
{
   if (!((!cubemapping_is_setup)))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::setup_cubemapping]: error: guard \"(!cubemapping_is_setup)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::setup_cubemapping: error: guard \"(!cubemapping_is_setup)\" not met");
   }
   if (!(std::filesystem::exists(cube_map_texture_filename)))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::setup_cubemapping]: error: guard \"std::filesystem::exists(cube_map_texture_filename)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::setup_cubemapping: error: guard \"std::filesystem::exists(cube_map_texture_filename)\" not met");
   }
   // TODO: Needs duplicate init guards
   AllegroFlare::Cubemap* cubemap = nullptr;
   AllegroFlare::CubemapBuilder builder;
   //std::string cube_map_texture_filename = get_fixtures_path() + "bitmaps/black_prism_1-01.png";
   cubemap = builder.glsl_create_cubemap_from_vertical_strip(cube_map_texture_filename.c_str());

   cubemap_shader.initialize();
   cubemap_shader.set_cube_map(cubemap);

   cubemapping_is_setup = true;
   return;
}

void SceneRenderer2::setup_multitexture_shader()
{
   if (!((!multitexture_shader_is_setup)))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::setup_multitexture_shader]: error: guard \"(!multitexture_shader_is_setup)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::setup_multitexture_shader: error: guard \"(!multitexture_shader_is_setup)\" not met");
   }
   multitexture_shader.initialize();
   multitexture_shader_is_setup = true;
   return;
}

AllegroFlare::Camera3D* SceneRenderer2::find_primary_camera_3d()
{
   Entities::Base *entity = entity_pool->find_with_attribute("primary_camera");
   if (!entity)
   {
      AllegroFlare::Logger::throw_error(
         "AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::SceneRenderer::primary_camera_3d",
         "no camera present"
      );
   }
   // TODO: validate the camera is of type Entities::Camera
   Entities::Camera3D *as_camera = static_cast<Entities::Camera3D*>(entity);
   return &as_camera->get_camera_3d_ref();
}

void SceneRenderer2::render()
{
   if (!(entity_pool))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::render]: error: guard \"entity_pool\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::render: error: guard \"entity_pool\" not met");
   }
   if (!(multitexture_shader_is_setup))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::render]: error: guard \"multitexture_shader_is_setup\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::render: error: guard \"multitexture_shader_is_setup\" not met");
   }
   if (!(render_surface_is_setup))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::render]: error: guard \"render_surface_is_setup\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::render: error: guard \"render_surface_is_setup\" not met");
   }
   if (!(cubemapping_is_setup))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::render]: error: guard \"cubemapping_is_setup\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::render: error: guard \"cubemapping_is_setup\" not met");
   }
   if (!(shadow_map_buffer_is_setup))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::render]: error: guard \"shadow_map_buffer_is_setup\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::render: error: guard \"shadow_map_buffer_is_setup\" not met");
   }
   AllegroFlare::Camera3D *primary_camera = find_primary_camera_3d();



   // Render the shadow_buffer_bitmap
   shadow_map_buffer.render();
   ALLEGRO_BITMAP* shadow_buffer_bitmap = shadow_map_buffer.get_result_bitmap();
   //al_save_bitmap("/Users/markoates/Desktop/shadow_buffer_bitmap.png", scene_renderer.shadow_map_buffer_ref().get_result_bitmap());





   using namespace AllegroFlare::GraphicsPipelines::DynamicEntityPipeline;
   //headers: [ AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/SceneRenderer.hpp ]

   // Extract out the camera and render the scene
   //Entities::Base *entity = entity_pool->find_with_attribute("primary_camera");
   //if (!entity) throw std::runtime_error("no camera present");
   //// TODO: validate the camera is of type Entities::Camera
   //Entities::Camera3D *as_camera = static_cast<Entities::Camera3D*>(entity);

   // TODO: Get a proper render surface, rather than pulling from the current display
   //ALLEGRO_BITMAP *render_surface_bmp = al_get_backbuffer(al_get_current_display()); // TODO: replace with render surface
   ALLEGRO_BITMAP *render_surface_bmp = render_surface.obtain_surface();

   al_set_target_bitmap(render_surface_bmp);


   al_clear_depth_buffer(1);
   al_clear_to_color(ALLEGRO_COLOR{0.1, 0.105, 0.12, 1.0});

   //AllegroFlare::Camera3D &camera = as_camera->get_camera_3d_ref();
   //AllegroFlare::Camera3D *primary_camera = find_primary_camera_3d();
   primary_camera->setup_projection_on(render_surface_bmp);

   // Set the camera position in the iridescent shder
   cubemap_shader.set_camera_position(primary_camera->get_real_position());

   //int entities_rendered = 0;

   //std::unordered_set<AllegroFlare::SceneGraph::Entities::Base*>
   for (auto &entity : entity_pool->get_entity_pool_ref())
   {
      // TODO: Rename legacy "as_agc_entity" to something more appropriate
      Entities::Base *as_agc_entity = static_cast<Entities::Base*>(entity);

      // Skip if entity is flagged as "do not render"
      // TODO: Add test for this case
      if (as_agc_entity->exists(EntityRenderFlags::DO_NOT_RENDER)) continue;
      

      AllegroFlare::Model3D *model = get_model_3d(as_agc_entity); //as_agc_entity->get_model();
      AllegroFlare::MultitextureModel3D *multitexture_model_3d = nullptr;

      if (model)
      {
         AllegroFlare::Placement3D *placement = get_placement_3d(as_agc_entity);
         if (!placement) continue; // TODO: Test this line

         // Collect render flags
         bool renders_with_iridescent = as_agc_entity->exists(EntityRenderFlags::RENDER_WITH_SKYBOX);

         // Setup the render for this object
         if (renders_with_iridescent)
         {
            //Gameplay::Entities::Base *as_gac_base = static_cast<Entities::Base*>(entity);

            // NOTE: For now, this has to be set before activating the shader
            cubemap_shader.set_object_placement(placement);

            cubemap_shader.activate();
         }
         else
         {
            ALLEGRO_BITMAP *texture = get_texture(as_agc_entity); //->get_texture();
            if (texture) model->set_texture(texture);
            placement->start_transform();
            //as_agc_entity->get_placement_ref().start_transform();
         }

         // Draw the model
         model->draw();
         //entities_rendered++;

         // Teardown the render for this object
         if (renders_with_iridescent)
         {
            cubemap_shader.deactivate();
         }
         else
         {
            placement->restore_transform();
         }
      }
      else if ((multitexture_model_3d = get_multitexture_model_3d(as_agc_entity)))
      {
         // Isolate our entity's type
         Entities::StaticMultitextureModel3D *as_multitexture_model_3d_entity =
            static_cast<Entities::StaticMultitextureModel3D*>(entity);

         // Extract out our textures
         ALLEGRO_BITMAP *texture_a = get_texture(as_agc_entity);
         if (texture_a) as_multitexture_model_3d_entity->set_multitexture_model_3d_texture_1(texture_a);
         else throw std::runtime_error("no texture_a"); // TODO: Improve this error message

         ALLEGRO_BITMAP *texture_b = get_texture_2(as_agc_entity);
         if (texture_b) as_multitexture_model_3d_entity->set_multitexture_model_3d_texture_2(texture_b);
         else throw std::runtime_error("no texture_b"); // TODO: Improve this error message


         // Assign the textures to the shader
         multitexture_shader.set_texture_a(texture_a);
         multitexture_shader.set_texture_b(texture_b);

         // Activate the shader
         multitexture_shader.activate();

         // Render our subject
         // NOTE: For this test, will not be using "subject.draw()". Instead we will be rendering manually, and
         // setting  textures on the shader manually
         std::vector<AllegroFlare::ALLEGRO_VERTEX_WITH_TWO_UVS_AND_NORMAL> &vertices =
            multitexture_model_3d->vertexes;

         al_draw_prim(
            &vertices[0],
            multitexture_model_3d->vertex_declaration,
            texture_a, // TODO: In this sloppy case, this texture is used to determine the
                       // dimensionality of the textures(s) of the shader. Note that at the time of this writing, the
                       // textures and dimensions are all inter-dependent on each other in this way. This kink
                       // should eventually be worked out and cleaned up.
            0,
            vertices.size(),
            ALLEGRO_PRIM_TRIANGLE_LIST
         );

         multitexture_shader.deactivate();
      }
      else // (!model) or (!multitexture_model)
      {
         ALLEGRO_BITMAP *texture = get_texture(as_agc_entity); //->get_texture();
         AllegroFlare::Placement3D *placement = get_placement_3d(as_agc_entity);

         if (texture)
         {
            placement->start_transform();
            al_draw_bitmap(texture, 0, 0, ALLEGRO_FLIP_VERTICAL);
            placement->restore_transform();
         }
      }
   }


   //std::cout << "Entities rendered: " << entities_rendered << std::endl;


   // 
   //al_set_target_bitmap(render_surface_bmp);

   al_clear_depth_buffer(1); // Consider just using a simple write mask
   //al_clear_to_color(ALLEGRO_COLOR{0.1, 0.105, 0.12, 1.0});
   //AllegroFlare::Camera2D camera2d;
   //camera2d.setup_dimensional_projection(render_surface_bmp);
   //ALLEGRO_TRANSFORM transform;
   //al_identity_transform(&transform);
   //al_use_transform(render_surface_bmp, &transform);



   // Basic shadow composite

   ALLEGRO_TRANSFORM trans;
   al_identity_transform(&trans);
   al_orthographic_transform(&trans, 0, 0, -1.0, al_get_bitmap_width(render_surface_bmp),
                             al_get_bitmap_height(render_surface_bmp), 1.0);

   //al_set_target_bitmap(bitmap);
   al_use_projection_transform(&trans);

   al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

   ALLEGRO_BITMAP *render_surface = shadow_buffer_bitmap;
   al_draw_tinted_scaled_bitmap(
      //shadow_depth_map_renderer.get_result_surface_bitmap(),
      //depth_pass.get_result_surface_bitmap(),
      render_surface,
      ALLEGRO_COLOR{1.0, 1.0, 1.0, 1.0},
      //ALLEGRO_COLOR{0.4, 0.4, 0.4, 0.4},
      0,
      0,
      al_get_bitmap_width(render_surface),
      al_get_bitmap_height(render_surface),
      0,
      0,
      al_get_bitmap_width(render_surface_bmp), //1920, // the surface width of the result    //al_get_display_width(get_display()),
      al_get_bitmap_height(render_surface_bmp), //1080, // the size of the result             //al_get_display_height(get_display()),
      //ALLEGRO_COLOR{1.0, 1.0, 1.0, 1.0},
      //ALLEGRO_COLOR{0.4, 0.4, 0.4, 0.4},
      //0,
      //0,
      0
   );

   al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);



   return;
}

AllegroFlare::MultitextureModel3D* SceneRenderer2::get_multitexture_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::get_multitexture_model_3d]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::get_multitexture_model_3d: error: guard \"entity\" not met");
   }
   using namespace AllegroFlare::GraphicsPipelines::DynamicEntityPipeline;

   // TODO: Optimize this lookup
   // TODO: Consider throw on unhandled type
   if (entity->is_type(Entities::StaticMultitextureModel3D::TYPE))
   {
      Entities::StaticMultitextureModel3D *as_casted = static_cast<Entities::StaticMultitextureModel3D*>(entity);
      return as_casted->get_multitexture_model_3d();
   }
   return nullptr;
}

AllegroFlare::Model3D* SceneRenderer2::get_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::get_model_3d]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::get_model_3d: error: guard \"entity\" not met");
   }
   // TODO: Optimize this lookup
   // TODO: Consider throw on unhandled type
   if (entity->is_type(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D::TYPE))
   {
      Entities::DynamicModel3D *as_casted = static_cast<Entities::DynamicModel3D*>(entity);
      return as_casted->get_model_3d();
   }
   else if (entity->is_type(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::StaticModel3D::TYPE))
   {
      Entities::StaticModel3D *as_casted = static_cast<Entities::StaticModel3D*>(entity);
      return as_casted->get_model_3d();
   }
   return nullptr;
}

AllegroFlare::Placement3D* SceneRenderer2::get_placement_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::get_placement_3d]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::get_placement_3d: error: guard \"entity\" not met");
   }
   // TODO: Optimize this lookup
   // TODO: Consider throw on unhandled type
   if (entity->is_type(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D::TYPE))
   {
      Entities::DynamicModel3D *as_casted = static_cast<Entities::DynamicModel3D*>(entity);
      return &as_casted->get_placement_ref();
   }
   else if (entity->is_type(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::StaticModel3D::TYPE))
   {
      Entities::StaticModel3D *as_casted = static_cast<Entities::StaticModel3D*>(entity);
      return &as_casted->get_placement_ref();
   }
   return nullptr;
}

ALLEGRO_BITMAP* SceneRenderer2::get_texture(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::get_texture]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::get_texture: error: guard \"entity\" not met");
   }
   using namespace AllegroFlare::GraphicsPipelines::DynamicEntityPipeline;

   // TODO: Optimize this lookup
   // TODO: Consider throw on unhandled type
   if (entity->is_type(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::DynamicModel3D::TYPE))
   {
      Entities::DynamicModel3D *as_casted = static_cast<Entities::DynamicModel3D*>(entity);
      return as_casted->get_model_3d_texture();
   }
   else if (entity->is_type(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::StaticModel3D::TYPE))
   {
      Entities::StaticModel3D *as_casted = static_cast<Entities::StaticModel3D*>(entity);
      return as_casted->get_model_3d_texture();
   }
   if (entity->is_type(Entities::StaticMultitextureModel3D::TYPE))
   {
      Entities::StaticMultitextureModel3D *as_casted = static_cast<Entities::StaticMultitextureModel3D*>(entity);
      return as_casted->get_multitexture_model_3d_texture_1();
   }
   return nullptr;
}

ALLEGRO_BITMAP* SceneRenderer2::get_texture_2(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[SceneRenderer2::get_texture_2]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SceneRenderer2::get_texture_2: error: guard \"entity\" not met");
   }
   using namespace AllegroFlare::GraphicsPipelines::DynamicEntityPipeline;

   // TODO: Optimize this lookup
   // TODO: Consider throw on unhandled type
   if (entity->is_type(Entities::StaticMultitextureModel3D::TYPE))
   {
      Entities::StaticMultitextureModel3D *as_casted = static_cast<Entities::StaticMultitextureModel3D*>(entity);
      return as_casted->get_multitexture_model_3d_texture_2();
   }
   //else if (entity->is_type(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::StaticModel3D::TYPE))
   //{
      //Entities::StaticModel3D *as_casted = static_cast<Entities::StaticModel3D*>(entity);
      //return as_casted->get_model_3d_texture();
   //}
   return nullptr;
}


} // namespace DynamicEntityPipeline
} // namespace GraphicsPipelines
} // namespace AllegroFlare


