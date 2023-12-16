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
            AllegroFlare::Shaders::ShadowMapping shadow_mapping_shader;
            int shadow_depth_map_surface_width;
            int shadow_depth_map_surface_height;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2 shadow_depth_map_renderer;
            int result_surface_width;
            int result_surface_height;
            AllegroFlare::RenderSurfaces::Bitmap result_surface;
            bool initialized;

         protected:


         public:
            ShadowMapBuffer(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool=nullptr);
            ~ShadowMapBuffer();

            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* get_entity_pool() const;
            int get_shadow_depth_map_surface_width() const;
            int get_shadow_depth_map_surface_height() const;
            int get_result_surface_width() const;
            int get_result_surface_height() const;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2 &get_shadow_depth_map_renderer_ref();
            AllegroFlare::RenderSurfaces::Bitmap &get_result_surface_ref();
            void set_entity_pool(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool=nullptr);
            void set_shadow_depth_map_surface_width(int width=1920);
            void set_shadow_depth_map_surface_height(int height=1920);
            void set_result_surface_width(int width=1920);
            void set_result_surface_height(int height=1080);
            AllegroFlare::Camera3D* get_light();
            void initialize();
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



