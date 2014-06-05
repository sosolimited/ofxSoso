ofxSoso
=======
Soso OpenFrameworks Addon
Copyright (C) 2013 Sosolimited

#### change key  
	  + added  
	  - removed  
	  / modified  


CORE
----
### ofxCircleObject
	5/30/2014, JN:
	/ destructor: added proper deletion call for circlePoints (an array of pointers).

### ofxDynamicPolygonObject
	5/30/2014, JN:
	/ destructor: added deletion calls for dynamicVerts (vector of pointers to structs). TBD if correctly done.

### ofxGridSystem
	5/30/2014, JN:
	/ destructor: added proper deletion call for baselines (array of pointers).

### ofxLetterTextObjectLetter
	5/30/2014, JN:
	/ destructor: added proper deletion call for charPointer (array of pointers).

### ofxLineStripObject
	5/30/2014, JN:
	/ destructor: added proper deletion calls for vertices (vector of pointers to structs). TBD if correctly done.

### ofxMessage
	5/30/2014, JN:
	/ destructor: added proper deletion calls for baseEndVals. Since it can be of different types (depending on the message type) we have to cast back from void* to its original type to delete it correctly.

### ofxObject
	5/30/2014, JN:
	/ destructor: used modern delete statements. Replaced the old backwards-decerementing loops with modern for(auto) loops. Clearing vector correctly now.

### ofxPolygonObject
	5/30/2014, JN:
	/ destructor: added proper deletion calls for vertices, textCoords, vertexColors (arrays of pointers).

### ofxQuadStripObject
	5/30/2014, JN:
	/ destructor: added proper deletion calls for vertices, textCoords, vertexColors (arrays of pointers).

### ofxScene
	5/30/2014, JN:
	/ destructor: added deletion of sortedObjects, onTopObjects (double pointers). TBD if correctly done.

### ofxStateMachine
	5/30/2014, JN:
	/ destructor: added proper deletion of curState, curTransition, states, transitions.

### ofxLetterTextObject
	5/30/2014, WT:
	/ constructor uses strings like ofxTextObject

### ofxScroller
	5/28/2014, AO:
	+ Added ofxScroller.cpp
	+ Added ofxScroller.h
	6/2/2014, AO:
	+ Added setColor function with 3 args
	/ Removed default from setColor with 4 args
	+ Integrated Maja's code for OF_SETCOLOR4
	
	

EXAMPLES
----
### Structure
 	5/29/2014, AO:
	/ Finished ofxScroller port
	/ Finished scrollerExample
	5/28/2014, AO:
	- Deleted example folder and app
	+ Added examples folder
	+ Added emptyExample
	+ Added basicExample
	+ Added scrollerExample
