attribute vec4 al_pos;
attribute vec4 al_color;
attribute vec2 al_texcoord;
uniform mat4 al_projview_matrix;
uniform bool al_use_tex_matrix;
uniform mat4 al_tex_matrix;
varying vec4 varying_color;
varying vec2 varying_texcoord;


uniform mat4 my_object_position_transform;
uniform mat4 my_depth_pass_transform;
varying vec4 my_glPosition_from_light;
varying vec2 my_shadow_pass_uv_texcoord;


void main()
{
  my_glPosition_from_light = my_depth_pass_transform * my_object_position_transform * al_pos;


 // Calculate the color and the texture coords
 varying_color = al_color;
 if (al_use_tex_matrix)
 {
   // The classic allegro texcoord
   vec4 uv = al_tex_matrix * vec4(al_texcoord, 0, 1);
   varying_texcoord = vec2(uv.x, uv.y);

   // The shadow_pass texcoord
   vec4 shadow_pass_uv = al_tex_matrix * vec4(my_glPosition_from_light.xy, 0, 1);
   my_shadow_pass_uv_texcoord = vec2(shadow_pass_uv.x, shadow_pass_uv.y);
 }
 else
 {
   my_shadow_pass_uv_texcoord = my_glPosition_from_light.xy;
   varying_texcoord = al_texcoord;
 }


 // Calculate the position
 gl_Position = al_projview_matrix * my_object_position_transform * al_pos;
 //gl_Position = my_depth_pass_transform * al_pos; // If you want to view the light perspective
}
