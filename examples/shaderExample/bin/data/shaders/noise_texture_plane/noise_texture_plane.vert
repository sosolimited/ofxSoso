#version 120

// Texture value we're passing in from oF
uniform sampler2DRect tex0;

varying vec2 texCoordVarying;

void main()
{
  // Get the texture's coordinates
  texCoordVarying = gl_MultiTexCoord0.xy;
  
  // Get the position of the vertex relative to the modelViewProjectionMatrix
  vec4 position = ftransform();
  vec4 modifiedPosition = position;
  
  // We need to scale up the values we get from the texture
  float scale = 100;
  
  // Get the red channel value from the texture
  // to use it as vertical displacement
  float displacementY = texture2DRect(tex0, texCoordVarying).x;
  
  // Use the displacement we created from the texture data
  // to modify the vertex position
  modifiedPosition.y += displacementY * scale;
  
  // This is the resulting vertex position
	gl_Position = modifiedPosition;
}