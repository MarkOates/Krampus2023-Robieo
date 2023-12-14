#pragma once


#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/Base.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityPool.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/ShadowDepthMapRenderer2.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/MultitextureModel3D.hpp>
#include <AllegroFlare/Placement3D.hpp>
#include <AllegroFlare/Shaders/Cubemap.hpp>
#include <AllegroFlare/Shaders/Multitexture.hpp>
#include <allegro5/allegro.h>


namespace AllegroFlare
{
   namespace GraphicsPipelines
   {
      namespace DynamicEntityPipeline
      {
         class SceneRenderer2
         {
         private:
            AllegroFlare::Shaders::Cubemap* cubemap_shader;
            AllegroFlare::Shaders::Multitexture* multitexture_shader;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* shadow_depth_map_renderer;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* depth_pass;

         protected:


         public:
            SceneRenderer2(AllegroFlare::Shaders::Cubemap* cubemap_shader=nullptr, AllegroFlare::Shaders::Multitexture* multitexture_shader=nullptr, AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool=nullptr);
            ~SceneRenderer2();

            void set_cubemap_shader(AllegroFlare::Shaders::Cubemap* cubemap_shader);
            void set_multitexture_shader(AllegroFlare::Shaders::Multitexture* multitexture_shader);
            void set_entity_pool(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool);
            void set_shadow_depth_map_renderer(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* shadow_depth_map_renderer);
            void set_depth_pass(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* depth_pass);
            AllegroFlare::Shaders::Cubemap* get_cubemap_shader() const;
            AllegroFlare::Shaders::Multitexture* get_multitexture_shader() const;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* get_entity_pool() const;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* get_shadow_depth_map_renderer() const;
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::ShadowDepthMapRenderer2* get_depth_pass() const;
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



