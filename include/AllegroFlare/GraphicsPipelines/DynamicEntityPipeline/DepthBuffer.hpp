#pragma once


#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/Entities/Base.hpp>
#include <AllegroFlare/GraphicsPipelines/DynamicEntityPipeline/EntityPool.hpp>
#include <AllegroFlare/MultitextureModel3D.hpp>
#include <AllegroFlare/RenderSurfaces/Bitmap.hpp>
#include <AllegroFlare/Shaders/Base.hpp>
#include <allegro5/allegro.h>
#include <string>


namespace AllegroFlare
{
   namespace GraphicsPipelines
   {
      namespace DynamicEntityPipeline
      {
         class DepthBuffer
         {
         public:
            static constexpr char* DEFAULT_DATA_PATH_FOR_SHADERS = (char*)"[unset-data_path_for_shaders]";

         private:
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool;
            std::string data_path_for_shaders;
            AllegroFlare::Shaders::Base* depth_map_shader;
            AllegroFlare::Camera3D casting_light;
            AllegroFlare::RenderSurfaces::Bitmap render_surface;
            bool render_surface_is_setup;
            bool shader_is_initialized;

         protected:


         public:
            DepthBuffer(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool=nullptr);
            ~DepthBuffer();

            void set_entity_pool(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* entity_pool);
            void set_casting_light(AllegroFlare::Camera3D casting_light);
            AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::EntityPool* get_entity_pool() const;
            std::string get_data_path_for_shaders() const;
            AllegroFlare::Camera3D get_casting_light() const;
            AllegroFlare::Camera3D &get_casting_light_ref();
            void set_data_path_for_shaders(std::string data_path_for_shaders=DEFAULT_DATA_PATH_FOR_SHADERS);
            ALLEGRO_BITMAP* get_result_surface_bitmap();
            void setup_result_surface_bitmap(int width=1920, int height=1080);
            bool data_path_for_shaders_is_default();
            void init_shader();
            void init_camera_defaults();
            void destroy();
            void render();
            void setup_projection_on_render_surface();
            AllegroFlare::MultitextureModel3D* get_multitexture_model_3d(AllegroFlare::GraphicsPipelines::DynamicEntityPipeline::Entities::Base* entity=nullptr);
            void render_multitexture_model_3d(AllegroFlare::MultitextureModel3D* multitexture_model_3d=nullptr);
         };
      }
   }
}



