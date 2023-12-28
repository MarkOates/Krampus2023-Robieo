#pragma once


#include <AllegroFlare/Physics/AABB3D.hpp>
#include <string>


namespace Pipeline
{
   namespace Gameplay
   {
      class LevelCameraZone
      {
      private:
         std::string name;
         AllegroFlare::Physics::AABB3D bounding_box;

      protected:


      public:
         LevelCameraZone(std::string name="[unset-name]", AllegroFlare::Physics::AABB3D bounding_box={});
         ~LevelCameraZone();

         void set_name(std::string name);
         void set_bounding_box(AllegroFlare::Physics::AABB3D bounding_box);
         std::string get_name() const;
         AllegroFlare::Physics::AABB3D get_bounding_box() const;
         AllegroFlare::Physics::AABB3D &get_bounding_box_ref();
      };
   }
}



