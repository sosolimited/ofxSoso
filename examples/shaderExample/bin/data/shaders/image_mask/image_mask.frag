#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect mask;

varying vec2 texCoordVarying;

void main()
{
  vec4 imageTxt = texture2DRect(tex0, texCoordVarying);
  //vec4 gTxt = texture2DRect(tex1, texCoordVarying);
  //vec4 bTxt = texture2DRect(tex2, texCoordVarying);
  vec4 mask = texture2DRect(mask, texCoordVarying);
  vec4 color = vec4(0, 0, 0, 0);
  
  //color = mask;
  
  //if (mask.r > 0.9f)
  //  color = imageTxt;
  //if (mask.r > 0.5)
  //  color = imageTxt;
  
  
  color = mix(color, imageTxt, mask.r);
  //color = mix(color, gTxt, mask.g);
  //color = mix(color, bTxt, mask.b);
  
  gl_FragColor = color;
}