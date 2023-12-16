#pragma once


#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/Base.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityPool.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/ShadowMapBuffer.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/MultitextureModel3D.hpp>
#include <AllegroFlare/Placement3D.hpp>
#include <AllegroFlare/RenderSurfaces/Bitmap.hpp>
#include <AllegroFlare/Shaders/Cubemap.hpp>
#include <AllegroFlare/Shaders/Multitexture.hpp>
#include <allegro5/allegro.h>
#include <string>


namespace AllegroFlare
{
   namespace GraphicsPipelines
   {
      namespace DynamicEntityPipeline
      {
         class SceneRenderer2
         {
         private:
            AllegroFlare::Shaders::Cubemap cubemap_shader;
            AllegroFlare::Shaders::Multitexture* multitexture_shader;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowMapBuffer shadow_map_buffer;
            AllegroFlare::RenderSurfaces::Bitmap render_surface;
            bool render_surface_is_setup;
            bool cubemapping_is_setup;

         protected:


         public:
            SceneRenderer2(AllegroFlare::Shaders::Multitexture* multitexture_shader=nullptr, AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool=nullptr);
            ~SceneRenderer2();

            void set_multitexture_shader(AllegroFlare::Shaders::Multitexture* multitexture_shader);
            void set_entity_pool(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool);
            AllegroFlare::Shaders::Multitexture* get_multitexture_shader() const;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* get_entity_pool() const;
            AllegroFlare::Shaders::Cubemap &get_cubemap_shader_ref();
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowMapBuffer &get_shadow_map_buffer_ref();
            AllegroFlare::RenderSurfaces::Bitmap &get_render_surface_ref();
            void setup_result_surface_bitmap(int width=1920, int height=1080);
            void setup_shadow_map_buffer();
            void setup_cubemapping(std::string cube_map_texture_filename="[unset-cube_map_texture_filename]");
            AllegroFlare::Camera3D* find_primary_camera_3d();
            void render();
            AllegroFlare::MultitextureModel3D* get_multitexture_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
            AllegroFlare::Model3D* get_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
            AllegroFlare::Placement3D* get_placement_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
            ALLEGRO_BITMAP* get_texture(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
            ALLEGRO_BITMAP* get_texture_2(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
         };
      }
   }
}



