#pragma once


#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/Base.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityPool.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/ShadowDepthMapRenderer2.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/MultitextureModel3D.hpp>
#include <AllegroFlare/Placement3D.hpp>
#include <AllegroFlare/RenderSurfaces/Bitmap.hpp>
#include <AllegroFlare/Shaders/ShadowMapping.hpp>
#include <allegro5/allegro.h>


namespace AllegroFlare
{
   namespace GraphicsPipelines
   {
      namespace DynamicEntityPipeline
      {
         class ShadowMapBuffer
         {
         private:
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool;
            AllegroFlare::Shaders::ShadowMapping* shadow_mapping_shader;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* shadow_depth_map_renderer;
            AllegroFlare::RenderSurfaces::Bitmap render_surface;
            bool initialized;

         protected:


         public:
            ShadowMapBuffer(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool=nullptr, AllegroFlare::Shaders::ShadowMapping* shadow_mapping_shader=nullptr);
            ~ShadowMapBuffer();

            void set_entity_pool(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool);
            void set_shadow_mapping_shader(AllegroFlare::Shaders::ShadowMapping* shadow_mapping_shader);
            void set_shadow_depth_map_renderer(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* shadow_depth_map_renderer);
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* get_entity_pool() const;
            AllegroFlare::Shaders::ShadowMapping* get_shadow_mapping_shader() const;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* get_shadow_depth_map_renderer() const;
            AllegroFlare::RenderSurfaces::Bitmap &get_render_surface_ref();
            void setup_result_surface_bitmap(int width=1920, int height=1080);
            void render();
            AllegroFlare::Camera3D* find_primary_camera_3d();
            AllegroFlare::MultitextureModel3D* get_multitexture_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
            AllegroFlare::Model3D* get_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
            AllegroFlare::Placement3D* get_placement_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
            ALLEGRO_BITMAP* get_texture(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
            ALLEGRO_BITMAP* get_texture_2(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
         };
      }
   }
}



