#version 120

varying vec2 texCoordVarying;

void main(void)
{
  // Keep vertex values the same
	texCoordVarying = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
  
}