#ifdef GL_ES
precision lowp float;
#endif
uniform sampler2D al_tex;
uniform bool al_use_tex;
uniform bool al_alpha_test;
uniform int al_alpha_func;
uniform float al_alpha_test_val;
varying vec4 varying_color;
varying vec2 varying_texcoord;


varying vec4 me__glPosition_from_light;
uniform sampler2D me__depth_pass_sampler;
varying vec2 me__shadow_pass_uv_texcoord; // The uv coordintate on the shadow_depth_pass_sampler texture that we will use for depth



void main()
{
  vec4 c;
  if (al_use_tex)
  {
    c = varying_color * texture2D(al_tex, varying_texcoord);
    //c = varying_color * texture2D(me__depth_pass_sampler, varying_texcoord); // If you want to see the depth_pass_sampler used as the texture on all the models
  }
  else
  {
    c = varying_color;
  }

  gl_FragColor = c;
}
