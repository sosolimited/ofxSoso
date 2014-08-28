In progress switch over to Cinder as a base for this framework.

## Motivation:
C++11 support.
Automatic memory management with std::shared_ptr.
Support for multi-lingual text, JSON, animation, etc from Cinder.

## Status/Todo

[x] Basic object types working with identical interfaces and minimal internal changes.  
[x] Replace manual memory management in scene graph with std::shared_ptr.  
[x] Namespace classes within soso:: instead of using a C-style prefix.  
[] Vector typography support.  
[] Video support with AVFPlayer based on Sam Kronick's work.  
[] Switch to OpenGL 3.3+ Core Profile (waiting for dev version of Cinder to become public)  

Compare Choreograph/ci::Timeline with message-based animations for performance, flexibility.

## Implementation notes
VectorFont will replace SosoTrueTypeFont. Implement as an array of triangulated letterforms (map of char->VboMesh). Refer to gl::TextureFont for glyph layout.

AVFVideoPlayer should be a simple modification of [Sam's AVF player](https://github.com/kronick/ofxAVFVideoPlayer).

## Possible Issues:
Sorting of alpha-blending elements depends on [getWindowCoords](src/soso/Object.cpp#L450-L474), which may not be correct.
