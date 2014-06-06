#version 120

varying vec4 globalColor;

// the time value is passed into the shader by the OF app.
uniform float time;
uniform float percentX;
uniform float percentY;

void main()
{
    // We leave the position the same
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  
  
    float r = 255;
    float g = 255;
    float b = 255;
  
  
  
    // in OpenGL 2.0 we must get the global color using the gl_Color command,
    // and store it in a globalColor (varying) that is passed to our frag shader.
    // please note that the frag shader also has a globalColor (varying),
    // and this is the standard way of passing data from the vertex shader to the frag shader.

  // Sets our pass-through variable to the color of the object
  // Note - this may not always correspond to object color in special cases
  
  // do a little manipulation with color
  
  
  globalColor = vec4(percentX, gl_Color.g, percentY, 1);
    
}