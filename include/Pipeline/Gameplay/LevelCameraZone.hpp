#pragma once


#include <AllegroFlare/Physics/AABB3D.hpp>


namespace Pipeline
{
   namespace Gameplay
   {
      class LevelCameraZone
      {
      private:
         AllegroFlare::Physics::AABB3D bounding_box;

      protected:


      public:
         LevelCameraZone(AllegroFlare::Physics::AABB3D bounding_box={});
         ~LevelCameraZone();

         void set_bounding_box(AllegroFlare::Physics::AABB3D bounding_box);
         AllegroFlare::Physics::AABB3D get_bounding_box() const;
         AllegroFlare::Physics::AABB3D &get_bounding_box_ref();
      };
   }
}



