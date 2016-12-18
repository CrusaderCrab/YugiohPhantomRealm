## Yu-Gi-Oh Forbidden Memories 2: the Phantom Realm

### Overview

This is the source code for a Yu-Gi-Oh fan-game which I wrote during the summer of 2014 (just after my first year of CS studies). It is a sequel to the PSX game [*Yugioh: Forbidden Memories*](http://ie.ign.com/games/yu-gi-oh-forbidden-memories/ps-488283). It is entirely written in c++ and is Windows only (as it makes use of windows.h) although it would not be too difficult to make it platform independent. You can see some example footage of the game [*here*](https://www.youtube.com/watch?v=1bn4HnzwNqM). You can also download a built Windows executable (which includes the card data and images) from [*this link*](https://drive.google.com/file/d/0B6-poY1j5gCTV2syemJGSHBidWs/view).  

### On Code Quality

This is definitely a work of quantity, rather then one of quality. Even when I wrote this back in 2014 I knew the code was terrible (which it is), however I wanted to finish the project by the end of summer so I did not have time to design really anything. My motto for this was *write first, keep writing later*. I do not suggest that you try to understand this code, it is completely uncommented and has no structure to it whatsoever. I put it up online just to show that I can finish a large project, not to give an idea of my coding level.

### Dependencies

There are a number of third-party libraries needed to build this program. They have been removed from this repository for obvious license violation reasons. However given below are some notes on how to restore the source code so that it successfully builds.

##### ThirdParty Folder

In the top directory there is a 'ThirdParty' folder that needs to contain four files that have been removed from it.
* lodepng.h and lodepng.cpp: A simple library for loading PNG files. They were written by lvandeve, and can be found [*here*](https://github.com/lvandeve/lodepng).
* objloader.hpp and objloader.cpp: Used to load in complex 3D models. They are given as part of a OpenGL tutorial and can be found [*here*](https://github.com/huamulan/OpenGL-tutorial/tree/master/common).

##### Larger Libraries

A number of larger libraries were also used that must be obtained before the program can be built. The 32-bit version of the following libraries were used to build the 32-bit variant, and the 64-bit (if avaliable) version was used for the 64-bit variant of the program.
* Irrklang 1.5 
* Qt 4.8.6 (Most likely any 4.8.* version would work also)
* GLM (OpenGL Mathematics) 0.9.4.6
* GLEW 1.12

Irrklang and Qt were dynamically linked to, but this is not necessary. The top folder of each of these libraries "include" folder should be included in the build path (e.g. #include <GL\glew.h> #include <QtOpenGL\qglwidget> #include <irrKlang.h> #include <gtx\transform.hpp> should all work).
