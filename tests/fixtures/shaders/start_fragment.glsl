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

void main()
{
  vec4 c;
  if (al_use_tex)
  {
    c = varying_color * texture2D(al_tex, varying_texcoord);
  }
  else
  {
    c = varying_color;
  }

  gl_FragColor = c;
}
