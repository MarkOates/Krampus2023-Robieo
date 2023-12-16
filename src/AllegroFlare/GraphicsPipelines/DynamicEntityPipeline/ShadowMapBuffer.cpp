

#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/ShadowMapBuffer.hpp>

#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/Camera3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/DynamicModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/StaticModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/StaticMultitextureModel3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityRenderFlags.hpp>
#include <AllegroFlare/Logger.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace AllegroFlare
{
namespace GraphicsPipelines
{
namespace DynamicEntityPipeline
{


ShadowMapBuffer::ShadowMapBuffer(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool)
   : entity_pool(entity_pool)
   , shadow_mapping_shader()
   , shadow_depth_map_surface_width(1920)
   , shadow_depth_map_surface_height(1080)
   , shadow_depth_map_renderer()
   , result_surface_width(1920)
   , result_surface_height(1080)
   , result_surface()
   , initialized(false)
{
}


ShadowMapBuffer::~ShadowMapBuffer()
{
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* ShadowMapBuffer::get_entity_pool() const
{
   return entity_pool;
}


int ShadowMapBuffer::get_shadow_depth_map_surface_width() const
{
   return shadow_depth_map_surface_width;
}


int ShadowMapBuffer::get_shadow_depth_map_surface_height() const
{
   return shadow_depth_map_surface_height;
}


int ShadowMapBuffer::get_result_surface_width() const
{
   return result_surface_width;
}


int ShadowMapBuffer::get_result_surface_height() const
{
   return result_surface_height;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2 &ShadowMapBuffer::get_shadow_depth_map_renderer_ref()
{
   return shadow_depth_map_renderer;
}


AllegroFlare::RenderSurfaces::Bitmap &ShadowMapBuffer::get_result_surface_ref()
{
   return result_surface;
}


void ShadowMapBuffer::set_entity_pool(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool)
{
   this->entity_pool = entity_pool;
   shadow_depth_map_renderer.set_entity_pool(entity_pool);
   // TODO: Test guard
   //this->result_surface_width = result_surface_width;
   return;
}

void ShadowMapBuffer::set_shadow_depth_map_surface_width(int width)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::set_shadow_depth_map_surface_width]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::set_shadow_depth_map_surface_width: error: guard \"(!initialized)\" not met");
   }
   if (!((width >= 320)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::set_shadow_depth_map_surface_width]: error: guard \"(width >= 320)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::set_shadow_depth_map_surface_width: error: guard \"(width >= 320)\" not met");
   }
   // TODO: Test guard
   this->shadow_depth_map_surface_width = shadow_depth_map_surface_width;
   return;
}

void ShadowMapBuffer::set_shadow_depth_map_surface_height(int height)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::set_shadow_depth_map_surface_height]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::set_shadow_depth_map_surface_height: error: guard \"(!initialized)\" not met");
   }
   if (!((height >= 320)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::set_shadow_depth_map_surface_height]: error: guard \"(height >= 320)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::set_shadow_depth_map_surface_height: error: guard \"(height >= 320)\" not met");
   }
   // TODO: Test guard
   this->shadow_depth_map_surface_height = shadow_depth_map_surface_height;
   return;
}

void ShadowMapBuffer::set_result_surface_width(int width)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::set_result_surface_width]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::set_result_surface_width: error: guard \"(!initialized)\" not met");
   }
   if (!((width >= 320)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::set_result_surface_width]: error: guard \"(width >= 320)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::set_result_surface_width: error: guard \"(width >= 320)\" not met");
   }
   // TODO: Test guard
   this->result_surface_width = result_surface_width;
   return;
}

void ShadowMapBuffer::set_result_surface_height(int height)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::set_result_surface_height]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::set_result_surface_height: error: guard \"(!initialized)\" not met");
   }
   if (!((height >= 240)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::set_result_surface_height]: error: guard \"(height >= 240)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::set_result_surface_height: error: guard \"(height >= 240)\" not met");
   }
   // TODO: Test guard
   this->result_surface_height = result_surface_height;
   return;
}

void ShadowMapBuffer::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::initialize: error: guard \"(!initialized)\" not met");
   }
   shadow_depth_map_renderer.setup_result_surface_bitmap(
      shadow_depth_map_surface_width, 
      shadow_depth_map_surface_height
   );
   shadow_depth_map_renderer.init_shader();
   shadow_depth_map_renderer.set_entity_pool(entity_pool);
   shadow_depth_map_renderer.init_camera_defaults(); // NOTE: The camera defaults seem to be weird

   shadow_mapping_shader.initialize();

   result_surface.set_surface_width(result_surface_width);
   result_surface.set_surface_height(result_surface_height);
   result_surface.set_multisamples(0);
   result_surface.set_depth(32);
   result_surface.initialize();

   initialized = true;

   // TODO: Add a teardown

   return;
}

void ShadowMapBuffer::render()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::render]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::render: error: guard \"initialized\" not met");
   }
   if (!(entity_pool))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::render]: error: guard \"entity_pool\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::render: error: guard \"entity_pool\" not met");
   }
   AllegroFlare::Camera3D *primary_camera = find_primary_camera_3d();

   // Draw the shadow_depth_map_render
   //if (shadow_depth_map_renderer)
   //{
      shadow_depth_map_renderer.render();
   //}


   using namespace AllegroFlare::GraphicsPipelines::DynamicEntityPipeline;

   ALLEGRO_BITMAP *result_surface_bmp = result_surface.obtain_surface();

   al_set_target_bitmap(result_surface_bmp);


   al_clear_depth_buffer(1);
   //al_clear_to_color(ALLEGRO_COLOR{0.1, 0.105, 0.12, 1.0});
   al_clear_to_color(ALLEGRO_COLOR{0.98 - 0.005, 0.986 - 0.005, 0.99 - 0.005, 1.0}); // NOTE This is only a non-white
                                                                                     // color to make it a little
                                                                                     // easier for debugging

   primary_camera->setup_projection_on(result_surface_bmp);





   // Activate the shader
   shadow_mapping_shader.activate();

   // Obtain our shadow_depth_map_render texture and light position
   ALLEGRO_BITMAP *shadow_depth_map_render = shadow_depth_map_renderer.get_result_surface_bitmap();
   AllegroFlare::Camera3D &light = shadow_depth_map_renderer.get_casting_light_ref();
   ALLEGRO_TRANSFORM transform;
   shadow_depth_map_renderer.setup_transform_for_light(&transform);

   // Set the uniforms on the shader
   shadow_mapping_shader.set_sampler("me__depth_pass_sampler", shadow_depth_map_render, 0);
   shadow_mapping_shader.set_mat4("me__depth_pass_transform", &transform);





   // Set the camera position in the iridescent shder
   //cubemap_shader->set_camera_position(primary_camera->get_real_position());

   for (auto &entity : entity_pool->get_entity_pool_ref())
   {
      // TODO: Rename legacy "as_agc_entity" to something more appropriate
      Entities::Base *as_agc_entity = static_cast<Entities::Base*>(entity);
      // Skip if entity is flagged as "do not render"
      // TODO: Add test for this case
      if (as_agc_entity->exists(EntityRenderFlags::DO_NOT_RENDER)) continue;
      

      ALLEGRO_TRANSFORM object_transform;

      //al_identity_transform(&object_transform);
      //shadow_mapping_shader->set_mat4("me__object_position_transform", &object_transform);


      // Extract the model type that is being rendered
      AllegroFlare::Model3D *model = get_model_3d(as_agc_entity);
      //AllegroFlare::MultitextureModel3D *multitexture_model_3d = nullptr;

      if (model)
      {
         AllegroFlare::Placement3D *placement = get_placement_3d(as_agc_entity);
         if (!placement) continue; // TODO: Test this line

         // Collect render flags
         //bool renders_with_iridescent = as_agc_entity->exists(EntityRenderFlags::RENDER_WITH_SKYBOX);

         // Pass along the transform to the shader
         placement->build_transform(&object_transform);
         shadow_mapping_shader.set_mat4("me__object_position_transform", &object_transform);

         // Draw the model (make sure the texture is nullptr, because the depth_pass_sampler is using unit 0)
         model->set_texture(nullptr);
         model->draw();
      }
      else if (get_multitexture_model_3d(as_agc_entity))
      {
         AllegroFlare::Logger::throw_error(
            "AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowMapBuffer::render",
            "models of type Multitexture3D are nor supported at this point in the pipeline. Please consider using "
               "a DynamicModel3D instead."
         );
      }
      else
      {
         // Other objects may pass through here, like the camera or 2D maps, other DynamicEntityPipelin entity types
      }
   }

   shadow_mapping_shader.deactivate();

   return;
}

AllegroFlare::Camera3D* ShadowMapBuffer::find_primary_camera_3d()
{
   Entities::Base *entity = entity_pool->find_with_attribute("primary_camera");
   if (!entity)
   {
      AllegroFlare::Logger::throw_error(
         "AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowMapBuffer::primary_camera_3d",
         "no camera present"
      );
   }
   // TODO: validate the camera is of type Entities::Camera
   Entities::Camera3D *as_camera = static_cast<Entities::Camera3D*>(entity);
   return &as_camera->get_camera_3d_ref();
}

AllegroFlare::MultitextureModel3D* ShadowMapBuffer::get_multitexture_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::get_multitexture_model_3d]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::get_multitexture_model_3d: error: guard \"entity\" not met");
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

AllegroFlare::Model3D* ShadowMapBuffer::get_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::get_model_3d]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::get_model_3d: error: guard \"entity\" not met");
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

AllegroFlare::Placement3D* ShadowMapBuffer::get_placement_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::get_placement_3d]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::get_placement_3d: error: guard \"entity\" not met");
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

ALLEGRO_BITMAP* ShadowMapBuffer::get_texture(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::get_texture]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::get_texture: error: guard \"entity\" not met");
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

ALLEGRO_BITMAP* ShadowMapBuffer::get_texture_2(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::get_texture_2]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::get_texture_2: error: guard \"entity\" not met");
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


