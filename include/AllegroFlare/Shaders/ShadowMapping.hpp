#pragma once


#include <AllegroFlare/Shaders/Base.hpp>
#include <string>


namespace AllegroFlare
{
   namespace Shaders
   {
      class ShadowMapping : public AllegroFlare::Shaders::Base
      {
      public:
         static constexpr char* TYPE = (char*)"AllegroFlare/Shaders/ShadowMapping";
         static constexpr char* vertex_source_filename = (char*)"/Users/markoates/Repos/Pipeline/tests/fixtures/shaders/simple_map_vertex_with_light.glsl";
         static constexpr char* fragment_source_filename = (char*)"/Users/markoates/Repos/Pipeline/tests/fixtures/shaders/simple_map_fragment_with_light.glsl";

      private:
         bool initialized;
         static std::string obtain_vertex_source();
         static std::string obtain_fragment_source();

      protected:


      public:
         ShadowMapping();
         ~ShadowMapping();

         void initialize();
         void activate();
      };
   }
}



