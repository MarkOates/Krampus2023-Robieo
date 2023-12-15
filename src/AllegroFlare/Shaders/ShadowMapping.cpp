

#include <AllegroFlare/Shaders/ShadowMapping.hpp>

#include <AllegroFlare/UsefulPHP.hpp>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace AllegroFlare
{
namespace Shaders
{


ShadowMapping::ShadowMapping()
   : AllegroFlare::Shaders::Base(AllegroFlare::Shaders::ShadowMapping::TYPE)
   , initialized(false)
{
}


ShadowMapping::~ShadowMapping()
{
}


void ShadowMapping::initialize()
{
   set_vertex_source_code(obtain_vertex_source());
   set_fragment_source_code(obtain_fragment_source());

   if (!initialized) AllegroFlare::Shaders::Base::initialize();
   initialized = true;
}

void ShadowMapping::activate()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapping::activate]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapping::activate: error: guard \"initialized\" not met");
   }
   AllegroFlare::Shaders::Base::activate();
}

std::string ShadowMapping::obtain_vertex_source()
{
   //std::cout << "######" << vertex_source_filename << "######" << std::endl;
   //std::cout << "######" << std::filesystem::exists(vertex_source_filename) << "######" << std::endl;
   //std::string filename_fragment =
      //"/Users/markoates/Repos/allegro_flare/bin/data/shaders/simple_map_fragment_with_light.glsl";
   //std::string filename_vertex =
      //"/Users/markoates/Repos/allegro_flare/bin/data/shaders/simple_map_vertex_with_light.glsl";
   return AllegroFlare::php::file_get_contents(vertex_source_filename);

   static const std::string source = R"DELIM(
     attribute vec4 al_pos;
     attribute vec4 al_color;
     attribute vec2 al_texcoord;
     uniform mat4 al_projview_matrix;
     uniform bool al_use_tex_matrix;
     uniform mat4 al_tex_matrix;
     varying vec4 varying_color;
     varying vec2 varying_texcoord;
     void main()
     {
       varying_color = al_color;
       if (al_use_tex_matrix) {
         vec4 uv = al_tex_matrix * vec4(al_texcoord, 0, 1);
         varying_texcoord = vec2(uv.x, uv.y);
       }
       else
         varying_texcoord = al_texcoord;
       gl_Position = al_projview_matrix * al_pos;
     }
   )DELIM";
   return source;
}

std::string ShadowMapping::obtain_fragment_source()
{
   if (!(std::filesystem::exists(fragment_source_filename)))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapping::obtain_fragment_source]: error: guard \"std::filesystem::exists(fragment_source_filename)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapping::obtain_fragment_source: error: guard \"std::filesystem::exists(fragment_source_filename)\" not met");
   }
   //std::string filename_fragment =
      //"/Users/markoates/Repos/allegro_flare/bin/data/shaders/simple_map_fragment_with_light.glsl";
   //std::string filename_vertex =
      //"/Users/markoates/Repos/allegro_flare/bin/data/shaders/simple_map_vertex_with_light.glsl";
   return AllegroFlare::php::file_get_contents(fragment_source_filename);

   static const std::string source = R"DELIM(
     #ifdef GL_ES
     precision lowp float;
     #endif
     uniform sampler2D al_tex;
     uniform bool al_use_tex;
     uniform bool al_alpha_test;
     uniform int al_alpha_func;
     uniform float al_alpha_test_val;
     varying vec4 varying_color;
     varying vec2 varying_texcoord;

     bool alpha_test_func(float x, int op, float compare);

     void main()
     {
       vec4 c;
       if (al_use_tex)
         c = varying_color * texture2D(al_tex, varying_texcoord);
       else
         c = varying_color;
       if (!al_alpha_test || alpha_test_func(c.a, al_alpha_func, al_alpha_test_val))
         gl_FragColor = c;
       else
         discard;
     }

     bool alpha_test_func(float x, int op, float compare)
     {
       if (op == 0) return false;
       else if (op == 1) return true;
       else if (op == 2) return x < compare;
       else if (op == 3) return x == compare;
       else if (op == 4) return x <= compare;
       else if (op == 5) return x > compare;
       else if (op == 6) return x != compare;
       else if (op == 7) return x >= compare;
       return false;
     }
   )DELIM";
   return source;
}


} // namespace Shaders
} // namespace AllegroFlare


