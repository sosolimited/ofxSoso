// Frag shader sets each pixel's color

#version 120

// This value comes in via vert shader
varying vec4 globalColor;


void main()
{
    // globalColor is being set inside the vertex shader.
    // the fragment color is then set to the globalColor.
    gl_FragColor = globalColor;

}