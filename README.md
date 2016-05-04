## TDR

Tsubakura's Danmaku Recombobulator

### Installation

These instructions are for Visual Studio 2015. For other compilers / IDEs, you're unfortunately on your own. Note that your C++ compiler must support `#pragma once`.

* Download GLFW, SOIL, GLEW, and GLM **(Make sure you get the 32-bit versions if you're building for 32 bits, and the 64-bit versions if you're building for 64)**
* Copy GLFW (glfw3.lib), SOIL (SOIL.lib), and GLEW (glew32s.lib) library files into a lib/ directory
* Copy their includes, plus GLM, into include/
* Get the GTK+ bundle from [here](https://github.com/hexchat/gtk-win32); copy the directory here and rename it gtk/
* Play around in Visual Studio!

### Looking Around

At the time of writing, TDR consists of two components:

* FileUtil: a library to settle encoding differences between Windows and other OSes
* AGL: a C++ wrapper around OpenGL.
* TDR: the engine itself (NYI)
* Some future scripting layer

Plus a few test components:

* AGLTest: test program for AGL.
* TDRTest: test program for TDR.

### Resource Credits

Sample images (crate, awesome face), used in AGL test, by [Learn OpenGL](http://www.learnopengl.com), licensed under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/).
