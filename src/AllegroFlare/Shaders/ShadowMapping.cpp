

#include <AllegroFlare/Shaders/ShadowMapping.hpp>

#include <AllegroFlare/Logger.hpp>
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
   , data_path_for_shaders("")
   , initialized(false)
{
}


ShadowMapping::~ShadowMapping()
{
}


void ShadowMapping::set_data_path_for_shaders(std::string data_path_for_shaders)
{
   this->data_path_for_shaders = data_path_for_shaders;
}


std::string ShadowMapping::get_data_path_for_shaders() const
{
   return data_path_for_shaders;
}


std::string ShadowMapping::full_path_to_vertex_source_file()
{
   if (!((!data_path_for_shaders.empty())))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapping::full_path_to_vertex_source_file]: error: guard \"(!data_path_for_shaders.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapping::full_path_to_vertex_source_file: error: guard \"(!data_path_for_shaders.empty())\" not met");
   }
   return data_path_for_shaders + std::string(vertex_source_filename);
}

std::string ShadowMapping::full_path_to_fragment_source_file()
{
   if (!((!data_path_for_shaders.empty())))
   {
      std::stringstream error_message;
      error_message << "[ShadowMapping::full_path_to_fragment_source_file]: error: guard \"(!data_path_for_shaders.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("ShadowMapping::full_path_to_fragment_source_file: error: guard \"(!data_path_for_shaders.empty())\" not met");
   }
   return data_path_for_shaders + std::string(fragment_source_filename);
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
   if (!std::filesystem::exists(full_path_to_vertex_source_file()))
   {
      AllegroFlare::Logger::throw_error(
         "AllegroFlare::Shaders::ShadowMapping::obtain_vertex_source",
         "Could not load file \"" + full_path_to_vertex_source_file() + "\""
      );
   }
   return AllegroFlare::php::file_get_contents(full_path_to_vertex_source_file());
}

std::string ShadowMapping::obtain_fragment_source()
{
   if (!std::filesystem::exists(full_path_to_fragment_source_file()))
   {
      AllegroFlare::Logger::throw_error(
         "AllegroFlare::Shaders::ShadowMapping::obtain_fragment_source",
         "Could not load file \"" + full_path_to_fragment_source_file() + "\""
      );
   }
   return AllegroFlare::php::file_get_contents(full_path_to_fragment_source_file());
}


} // namespace Shaders
} // namespace AllegroFlare


