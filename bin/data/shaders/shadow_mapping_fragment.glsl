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


varying vec4 my_glPosition_from_light;
uniform sampler2D my_depth_pass_sampler;
varying vec2 my_shadow_pass_uv_texcoord; // The uv coordintate on the shadow_depth_pass_sampler texture that we will use for depth



void main()
{
  // Calculate the basic texture color
  vec4 c;
  if (al_use_tex)
  {
    c = varying_color * texture2D(al_tex, varying_texcoord);
    //c = varying_color * texture2D(my_depth_pass_sampler, varying_texcoord); // If you want to see the depth_pass_sampler used as the texture on all the models
  }
  else
  {
    c = varying_color;
  }


  // Calculate light
  bool in_casted_shadow = false;
  float bias = 0.0035;

  float vertex_dist_from_light = my_glPosition_from_light.z;
  //vec2 corrected_shadow_pass_uv = vec2(my_shadow_pass_uv_texcoord.x, -my_shadow_pass_uv_texcoord.y) * 0.5 + vec2(0.5, 0.5);
  vec2 corrected_shadow_pass_uv = vec2(my_shadow_pass_uv_texcoord.x, my_shadow_pass_uv_texcoord.y) * 0.5 + vec2(0.5, 0.5);
  //vec2 corrected_shadow_pass_uv = vec2(my_shadow_pass_uv_texcoord.x, -my_shadow_pass_uv_texcoord.y) * 10.0;
  vec4 sample_from_depth_pass = texture2D(my_depth_pass_sampler, corrected_shadow_pass_uv);
  if (sample_from_depth_pass.r < vertex_dist_from_light-bias) in_casted_shadow = true;

  if (in_casted_shadow)
  {
    float opacity = 0.5;
    c = vec4(
        c.r*opacity,
        c.g*opacity,
        c.b*opacity,
        1.0);
  }



  gl_FragColor = c;
}
