

#include <Pipeline/SmoothCamera.hpp>

#include <cmath>


namespace Pipeline
{


SmoothCamera::SmoothCamera()
   : camera_start({})
   , camera_end({})
   , time_starting_at(0.0f)
   , time_ending_at(1.0f)
   , time_now(0.5f)
{
}


SmoothCamera::~SmoothCamera()
{
}


void SmoothCamera::set_camera_start(AllegroFlare::Camera3D camera_start)
{
   this->camera_start = camera_start;
}


void SmoothCamera::set_camera_end(AllegroFlare::Camera3D camera_end)
{
   this->camera_end = camera_end;
}


void SmoothCamera::set_time_starting_at(float time_starting_at)
{
   this->time_starting_at = time_starting_at;
}


void SmoothCamera::set_time_ending_at(float time_ending_at)
{
   this->time_ending_at = time_ending_at;
}


void SmoothCamera::set_time_now(float time_now)
{
   this->time_now = time_now;
}


AllegroFlare::Camera3D SmoothCamera::get_camera_start() const
{
   return camera_start;
}


AllegroFlare::Camera3D SmoothCamera::get_camera_end() const
{
   return camera_end;
}


float SmoothCamera::get_time_starting_at() const
{
   return time_starting_at;
}


float SmoothCamera::get_time_ending_at() const
{
   return time_ending_at;
}


float SmoothCamera::get_time_now() const
{
   return time_now;
}


AllegroFlare::Camera3D &SmoothCamera::get_camera_start_ref()
{
   return camera_start;
}


AllegroFlare::Camera3D &SmoothCamera::get_camera_end_ref()
{
   return camera_end;
}


AllegroFlare::Camera3D SmoothCamera::update()
{
   AllegroFlare::Camera3D result = camera_start;
   float normal = normalize_age(time_starting_at, time_ending_at, time_now);

   if (normal <= 0.0) return camera_start;
   if (normal >= 1.0) return camera_end;

   result.near_plane = interpolate(camera_start.near_plane, camera_end.near_plane, normal);
   result.far_plane = interpolate(camera_start.far_plane, camera_end.far_plane, normal);
   result.position = interpolate_vec3d(camera_start.position, camera_end.position, normal);
   result.stepout = interpolate_vec3d(camera_start.stepout, camera_end.stepout, normal);
   result.spin = interpolate(camera_start.spin, camera_end.spin, normal);
   result.tilt = interpolate(camera_start.tilt, camera_end.tilt, normal);
   result.roll = interpolate(camera_start.roll, camera_end.roll, normal);
   result.zoom = interpolate(camera_start.zoom, camera_end.zoom, normal);

   //AllegroFlare::Camera3D

   // camera and viewport settings
   //float near_plane;
   //float far_plane;

   // camera controls
   //AllegroFlare::Vec3D position;
   //AllegroFlare::Vec3D stepout;
   //float spin;
   //float tilt;
   //float roll;
   //float zoom;
   return result;
}

float SmoothCamera::normalize_age(float start_time, float end_time, float time_now)
{
   // TODO: Consider that capping to 1.0 if past may not be preferred
   float length = (end_time - start_time);
   float time_now_in_range = time_now - start_time;
   if (length == 0.0f) return 0.0f;
   if (time_now_in_range >= length) return 1.0f;
   return time_now_in_range / length;
}

bool SmoothCamera::strobe(float start_time, float time_now, float strobes_per_second)
{
   float strobe_frequency = 1.0 / strobes_per_second;
   float age = (time_now - start_time);
   return (fmod(age, strobe_frequency) < (strobe_frequency * 0.5));
}

bool SmoothCamera::smooth_strobe(float start_time, float time_now, float strobes_per_second)
{
   // TODO: Update this method to actually be smooth (it's a copy of "strobe")
   float strobe_frequency = 1.0 / strobes_per_second;
   float age = (time_now - start_time);
   return (fmod(age, strobe_frequency) < (strobe_frequency * 0.5));
}

float SmoothCamera::age(float start_time, float end_time)
{
   return (end_time - start_time);
}

float SmoothCamera::interpolate(float start_value, float end_value, float normalized_float)
{
   return start_value + (normalized_float * (end_value - start_value));
}

AllegroFlare::Vec3D SmoothCamera::interpolate_vec3d(AllegroFlare::Vec3D start, AllegroFlare::Vec3D end, float normalized_float)
{
   AllegroFlare::Vec3D result;
   result.x = interpolate(start.x, end.x, normalized_float);
   result.y = interpolate(start.y, end.y, normalized_float);
   result.z = interpolate(start.z, end.z, normalized_float);
   return result;
}


} // namespace Pipeline


