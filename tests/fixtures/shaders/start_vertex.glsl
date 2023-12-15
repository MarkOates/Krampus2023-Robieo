attribute vec4 al_pos;
attribute vec4 al_color;
attribute vec2 al_texcoord;
uniform mat4 al_projview_matrix;
uniform bool al_use_tex_matrix;
uniform mat4 al_tex_matrix;
varying vec4 varying_color;
varying vec2 varying_texcoord;


uniform mat4 me__depth_pass_transform;
varying vec4 me__glPosition_from_light;


void main()
{
 varying_color = al_color;
 if (al_use_tex_matrix) {
   vec4 uv = al_tex_matrix * vec4(al_texcoord, 0, 1);
   varying_texcoord = vec2(uv.x, uv.y);
 }
 else
   varying_texcoord = al_texcoord;

 gl_Position = al_projview_matrix * al_pos;
 //gl_Position = me__depth_pass_transform * al_pos; // If you want to view the light perspective
}
