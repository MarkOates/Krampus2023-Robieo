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
         static constexpr char* vertex_source_filename = (char*)"start_vertex.glsl";
         static constexpr char* fragment_source_filename = (char*)"start_fragment.glsl";
         static constexpr char* TYPE = (char*)"AllegroFlare/Shaders/ShadowMapping";

      private:
         std::string data_path_for_shaders;
         bool initialized;
         std::string obtain_vertex_source();
         std::string obtain_fragment_source();

      protected:


      public:
         ShadowMapping();
         ~ShadowMapping();

         void set_data_path_for_shaders(std::string data_path_for_shaders);
         std::string get_data_path_for_shaders() const;
         std::string full_path_to_vertex_source_file();
         std::string full_path_to_fragment_source_file();
         void initialize();
         void activate();
      };
   }
}



