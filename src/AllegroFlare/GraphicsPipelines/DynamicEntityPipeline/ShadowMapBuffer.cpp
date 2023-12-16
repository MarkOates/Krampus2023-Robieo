

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


ShadowMapBuffer::ShadowMapBuffer(AllegroFlare::Shaders::Cubemap* cubemap_shader, AllegroFlare::Shaders::Multitexture* multitexture_shader, AllegroFlare::Shaders::ShadowMapping* shadow_mapping_shader, AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool)
   : cubemap_shader(cubemap_shader)
   , multitexture_shader(multitexture_shader)
   , shadow_mapping_shader(shadow_mapping_shader)
   , entity_pool(entity_pool)
   , shadow_depth_map_renderer(nullptr)
   , render_surface()
   , render_surface_is_setup(false)
{
}


ShadowMapBuffer::~ShadowMapBuffer()
{
}


void ShadowMapBuffer::set_cubemap_shader(AllegroFlare::Shaders::Cubemap* cubemap_shader)
{
   this->cubemap_shader = cubemap_shader;
}


void ShadowMapBuffer::set_multitexture_shader(AllegroFlare::Shaders::Multitexture* multitexture_shader)
{
   this->multitexture_shader = multitexture_shader;
}


void ShadowMapBuffer::set_shadow_mapping_shader(AllegroFlare::Shaders::ShadowMapping* shadow_mapping_shader)
{
   this->shadow_mapping_shader = shadow_mapping_shader;
}


void ShadowMapBuffer::set_entity_pool(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool)
{
   this->entity_pool = entity_pool;
}


void ShadowMapBuffer::set_shadow_depth_map_renderer(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* shadow_depth_map_renderer)
{
   this->shadow_depth_map_renderer = shadow_depth_map_renderer;
}


AllegroFlare::Shaders::Cubemap* ShadowMapBuffer::get_cubemap_shader() const
{
   return cubemap_shader;
}


AllegroFlare::Shaders::Multitexture* ShadowMapBuffer::get_multitexture_shader() const
{
   return multitexture_shader;
}


AllegroFlare::Shaders::ShadowMapping* ShadowMapBuffer::get_shadow_mapping_shader() const
{
   return shadow_mapping_shader;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* ShadowMapBuffer::get_entity_pool() const
{
   return entity_pool;
}


AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* ShadowMapBuffer::get_shadow_depth_map_renderer() const
{
   return shadow_depth_map_renderer;
}


AllegroFlare::RenderSurfaces::Bitmap &ShadowMapBuffer::get_render_surface_ref()
{
   return render_surface;
}


void ShadowMapBuffer::setup_result_surface_bitmap(int width, int height)
{
   render_surface.set_surface_width(width);
   render_surface.set_surface_height(height);
   render_surface.set_multisamples(0);
   render_surface.set_depth(32);
   render_surface.initialize();

   render_surface_is_setup = true;

   return;
}

AllegroFlare::Camera3D* ShadowMapBuffer::find_primary_camera_3d()
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

void ShadowMapBuffer::render()
{
   if (!(entity_pool))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::render]: error: guard \"entity_pool\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::render: error: guard \"entity_pool\" not met");
   }
   if (!(cubemap_shader))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::render]: error: guard \"cubemap_shader\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::render: error: guard \"cubemap_shader\" not met");
   }
   if (!(multitexture_shader))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::render]: error: guard \"multitexture_shader\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::render: error: guard \"multitexture_shader\" not met");
   }
   if (!(shadow_mapping_shader))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::render]: error: guard \"shadow_mapping_shader\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::render: error: guard \"shadow_mapping_shader\" not met");
   }
   if (!(shadow_depth_map_renderer))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::render]: error: guard \"shadow_depth_map_renderer\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::render: error: guard \"shadow_depth_map_renderer\" not met");
   }
   if (!(render_surface_is_setup))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapBuffer::render]: error: guard \"render_surface_is_setup\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapBuffer::render: error: guard \"render_surface_is_setup\" not met");
   }
   AllegroFlare::Camera3D *primary_camera = find_primary_camera_3d();

   // Draw the shadow_depth_map_render
   if (shadow_depth_map_renderer)
   {
      shadow_depth_map_renderer->render();
   }


   using namespace AllegroFlare::GraphicsPipelines::DynamicEntityPipeline;

   ALLEGRO_BITMAP *render_surface_bmp = render_surface.obtain_surface();

   al_set_target_bitmap(render_surface_bmp);


   al_clear_depth_buffer(1);
   al_clear_to_color(ALLEGRO_COLOR{0.1, 0.105, 0.12, 1.0});

   primary_camera->setup_projection_on(render_surface_bmp);





   // Activate the shader
   shadow_mapping_shader->activate();

   // Obtain our shadow_depth_map_render texture and light position
   ALLEGRO_BITMAP *shadow_depth_map_render = shadow_depth_map_renderer->get_result_surface_bitmap();
   AllegroFlare::Camera3D &light = shadow_depth_map_renderer->get_casting_light_ref();
   ALLEGRO_TRANSFORM transform;
   shadow_depth_map_renderer->setup_transform_for_light(&transform);

   // Set the uniforms on the shader
   shadow_mapping_shader->set_sampler("me__depth_pass_sampler", shadow_depth_map_render);
   shadow_mapping_shader->set_mat4("me__depth_pass_transform", &transform);





   // Set the camera position in the iridescent shder
   //cubemap_shader->set_camera_position(primary_camera->get_real_position());

   for (auto &entity : entity_pool->get_entity_pool_ref())
   {
      // TODO: Rename legacy "as_agc_entity" to something more appropriate
      Entities::Base *as_agc_entity = static_cast<Entities::Base*>(entity);
      // Skip if entity is flagged as "do not render"
      // TODO: Add test for this case
      if (as_agc_entity->exists(EntityRenderFlags::DO_NOT_RENDER)) continue;
      

      ALLEGRO_TRANSFORM object_transform; // TODO: Consider alternative, add clarity if the
                                          // object performs the transform or if the objects placement is
                                          // passed along to the shader. You will need to update/align all the
                                          // shaders/renderers together
      al_identity_transform(&object_transform);
      shadow_mapping_shader->set_mat4("me__object_position_transform", &object_transform);


      // Extract the model type that is being rendered
      AllegroFlare::Model3D *model = get_model_3d(as_agc_entity);
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
            // NOTE: For now, this has to be set before activating the shader
            //cubemap_shader->set_object_placement(placement);
            //cubemap_shader->activate();
            ALLEGRO_TRANSFORM object_transform;
            placement->build_transform(&object_transform);
            shadow_mapping_shader->set_mat4("me__object_position_transform", &object_transform);
         }
         else
         {
            ALLEGRO_BITMAP *texture = get_texture(as_agc_entity);
            if (texture) model->set_texture(texture);
            placement->start_transform();
         }

         // Draw the model
         model->draw();

         // Teardown the render for this object
         if (renders_with_iridescent)
         {
            //cubemap_shader->deactivate();
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
         multitexture_shader->set_texture_a(texture_a);
         multitexture_shader->set_texture_b(texture_b);

         // Activate the shader
         //multitexture_shader->activate();

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

         //multitexture_shader->deactivate();
      }
      else // (!model) or (!multitexture_model)
      {
         ALLEGRO_BITMAP *texture = get_texture(as_agc_entity);
         AllegroFlare::Placement3D *placement = get_placement_3d(as_agc_entity);

         if (texture)
         {
            placement->start_transform();
            al_draw_bitmap(texture, 0, 0, ALLEGRO_FLIP_VERTICAL);
            placement->restore_transform();
         }
      }
   }

   shadow_mapping_shader->deactivate();

   return;
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


