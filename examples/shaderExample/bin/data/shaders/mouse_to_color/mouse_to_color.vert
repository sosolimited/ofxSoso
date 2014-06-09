// Vec shader manipulates the position of vertices
// It also passes 

#version 120

// We'll pass this variable from vert shader to frag shader
varying vec4 globalColor;

// Declare variables that we'll pass in via oF app
uniform float percentX;
uniform float percentY;

void main()
{
    // Leave the vertex position the same (still have to set this)
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    // We're passing this value to our frag shader
    globalColor = vec4(percentX, gl_Color.g, percentY, 1); // These are all 0-1 values
    
}