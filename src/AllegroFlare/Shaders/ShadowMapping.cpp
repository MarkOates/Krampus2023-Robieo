

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
   return AllegroFlare::php::file_get_contents(vertex_source_filename);
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
   return AllegroFlare::php::file_get_contents(fragment_source_filename);
}


} // namespace Shaders
} // namespace AllegroFlare


