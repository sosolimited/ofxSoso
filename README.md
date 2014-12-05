ofxSoso
=======
Soso OpenFrameworks Addon
Copyright (C) 2012-2014 Sosolimited


OpenFrameworks addon that provides a scene graph structure, animations, enhanced text, and more.  Look at src header files for descriptions of each object.

As of 12/2014, we will tag ofxSoso releases with names corresponding to openFrameworks releases.  We recommend grabbing the oF and ofxSoso release versions for best results. 

While we strive to maintain compatibility between the ofxSoso master branch and the oF master branch, only release versions are guaranteed.


Basic instructions
------------

1 - Clone OpenFrameworks 
```git clone https://github.com/openframeworks/openframeworks.git```  

2 - Clone ofxSoso in openFrameworks/addons  
```git clone https://github.com/sosolimited/ofxSoso.git```  


For versions of openFrameworks 0.8.4 and earlier:
------------

3 - Clone the Sosolimited fork of ofxAVFVideoPlayer into openFrameworks/addons  
```git clone https://github.com/sosolimited/ofxAVFVideoPlayer.git```

4 - Follow kronick’s instructions for adding ofxAVFVideoPlayer to your project


To use release versions of oF and ofxSoso (recommended)
------------
Fetch tags for both openFrameworks and ofxSoso git repos.  
```git fetch — tags```  

Checkout matching  
```git fetch — tags``` 


List tags for openFrameworks and checkout appropriate version.  
```git tag -l```  
```git checkout 0.8.4```  

List tags for ofxSoso  and checkout appropriate version.  
```git tag -l```  
```git checkout 0.8.4.0```  


To use master versions of oF and ofxSoso
------------
No additional work is needed.


Creating a New Example Project
------------
To create a new example project, duplicate the empty example found in ofxSoso/examples.  Rename the containing folder after duplication, but NONE of the project files.

To rename the project files within XCode 5 or above, slowly double click on the project's name in the project explorer.
Type in your new project's name and  when prompted, agree to rename all other relevant files. 

Note:  You will manually have to create new schemes to match your new project name.


Examples
------------
Most ofxSoso examples run without additional addons.

To run the addonGuiExample, download ofxUI and place in openFrameworks/addons.


Notes
-----
Examples are currently built and tested with Xcode. Visual Studio support is untested.




