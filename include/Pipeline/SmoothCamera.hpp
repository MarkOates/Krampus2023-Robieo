#pragma once


#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/Vec3D.hpp>


namespace Pipeline
{
   class SmoothCamera
   {
   private:
      AllegroFlare::Camera3D camera_start;
      AllegroFlare::Camera3D camera_end;
      float time_starting_at;
      float time_ending_at;
      float time_now;

   protected:


   public:
      SmoothCamera();
      ~SmoothCamera();

      void set_camera_start(AllegroFlare::Camera3D camera_start);
      void set_camera_end(AllegroFlare::Camera3D camera_end);
      void set_time_starting_at(float time_starting_at);
      void set_time_ending_at(float time_ending_at);
      void set_time_now(float time_now);
      AllegroFlare::Camera3D get_camera_start() const;
      AllegroFlare::Camera3D get_camera_end() const;
      float get_time_starting_at() const;
      float get_time_ending_at() const;
      float get_time_now() const;
      AllegroFlare::Camera3D &get_camera_start_ref();
      AllegroFlare::Camera3D &get_camera_end_ref();
      float get_normalized_time();
      AllegroFlare::Camera3D update();
      static float normalize_age(float start_time=0.0f, float end_time=0.0f, float time_now=0.0f);
      static bool strobe(float start_time=0.0f, float time_now=0.0f, float strobes_per_second=10.0f);
      static bool smooth_strobe(float start_time=0.0f, float time_now=0.0f, float strobes_per_second=10.0f);
      static float age(float start_time=0.0f, float end_time=0.0f);
      static float interpolate(float start_value=0.0f, float end_value=0.0f, float normalized_float=0.0f);
      static AllegroFlare::Vec3D interpolate_vec3d(AllegroFlare::Vec3D start={}, AllegroFlare::Vec3D end={}, float normalized_float=0.0f);
   };
}



