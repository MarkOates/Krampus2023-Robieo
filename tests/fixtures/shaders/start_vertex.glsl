attribute vec4 al_pos;
attribute vec4 al_color;
attribute vec2 al_texcoord;
uniform mat4 al_projview_matrix;
uniform bool al_use_tex_matrix;
uniform mat4 al_tex_matrix;
varying vec4 varying_color;
varying vec2 varying_texcoord;


uniform mat4 me__object_position_transform;
uniform mat4 me__depth_pass_transform;
varying vec4 me__glPosition_from_light;
varying vec2 me__shadow_pass_uv_texcoord;


void main()
{
  me__glPosition_from_light = me__depth_pass_transform * me__object_position_transform * al_pos;


 // Calculate the color and the texture coords
 varying_color = al_color;
 if (al_use_tex_matrix)
 {
   // The classic allegro texcoord
   vec4 uv = al_tex_matrix * vec4(al_texcoord, 0, 1);
   varying_texcoord = vec2(uv.x, uv.y);

   // The shadow_pass texcoord
   vec4 shadow_pass_uv = al_tex_matrix * vec4(me__glPosition_from_light.xy, 0, 1);
   me__shadow_pass_uv_texcoord = vec2(shadow_pass_uv.x, shadow_pass_uv.y);
 }
 else
 {
   varying_texcoord = al_texcoord;
 }


 // Calculate the position
 gl_Position = al_projview_matrix * me__object_position_transform * al_pos;
 //gl_Position = me__depth_pass_transform * al_pos; // If you want to view the light perspective
}
