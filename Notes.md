In progress switch over to Cinder as a base for this framework.

## Motivation:
C++11 support in openFrameworks is a ways out.
Cinder generally has more solid architecture, support for Json, etc. in core (not addons)

## Initial goal:
All pieces working with identical interfaces and minimal internal changes.

## Next goal:
Replace all manual memory management with std::shared_ptr
Replace what can be with libc++ functions.

## Later goal:
Switch to OpenGL 3.3 Core Profile (when cinder_next hits shelves in a month)
Compare Choreograph/ci::Timeline with message-based animations for performance, flexibility.

## Things that will be interesting:
VectorFont to replace of(xSoso)TrueTypeFont.
Mostly use ci::gl::TextureFont for that role, but for actual vector text, we need a drop-in.
It's basically an array of Path2ds, which are based off of Cinder Path2d anyway.

AVFVideoPlayer
Should be a simple modification of Sam's work.

Working GUI replacement
Look at Simon's ImGUI wrapper for Cinder.
AntTweakBar (because it's really okay when it comes down to it, but not pretty)
Write our own...
