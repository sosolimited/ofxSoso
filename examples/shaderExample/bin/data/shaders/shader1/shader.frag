#version 120

varying vec4 globalColor;


void main()
{
    // globalColor is being set inside the vertex shader.
    // the fragment color is then set to the globalColor.

    gl_FragColor = globalColor;
  //gl_FragColor = gl_FragColor;
  //gl_FragColor = vec4(1,0,0,1);

}