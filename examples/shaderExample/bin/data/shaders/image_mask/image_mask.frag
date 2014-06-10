#version 120

// Values we'll pass in via oF
// In this case, we're passing textures
uniform sampler2DRect tex0;
uniform sampler2DRect mask;

varying vec2 texCoordVarying;

void main()
{
  // Get image and mask values of this coordinate
  vec4 imageTxt = texture2DRect(tex0, texCoordVarying);
  vec4 mask = texture2DRect(mask, texCoordVarying);
  
  // Create color variable
  vec4 color = vec4(0, 0, 0, 0);

  // Where our mask is red, mix in the image texture
  color = mix(color, imageTxt, mask.r);

  // Set the color
  gl_FragColor = color;
}