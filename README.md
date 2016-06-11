## TDR

Tsubakura's Danmaku Recombobulator

### Installation

For other compilers / IDEs, you're unfortunately on your own. Note that your C++ compiler must support `#pragma once`.

#### CMake (recommended)

* Make sure you have the following dependencies:
  * Those that can be installed by installing GTK2: GObject, GLib, Cairo, Pango, PangoCairo
  * OpenGL-related stuff: OpenGL itself, GLFW, GLEW, SOIL, and GLM
  * CMake, to build TDR itself
* Clone this repository
* Navigate to the directory that you just cloned
* Type `cmake .` followed by `make` (if you want to use `make`, that is)
* Note: the build system assumes that you are running the test executables while your PWD is the `build/` directory. So you can just temporarily CD there to run the test programs.

To get the dependencies with `apt-get`:

    sudo apt-get install libglfw3-dev libglew-dev libsoil-dev libglm-dev \
      libcairo2-dev libpango1.0-dev libglib2.0-dev cmake

#### Visual Studio (deprecated)

* Download GLFW, SOIL, GLEW, and GLM **(Make sure you get the 32-bit versions if you're building for 32 bits, and the 64-bit versions if you're building for 64)**
* Copy GLFW (glfw3.lib), SOIL (SOIL.lib), and GLEW (glew32s.lib) library files into a lib/ directory
* Copy their includes, plus GLM, into include/ (place SOIL.h in include/SOIL/)
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

### License

This project uses [GTK+2](http://www.gtk.org/), licensed under the [GNU LGPL version 2.1](http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html).

TDR itself is licensed under the MIT license; see `LICENSE` for details. As far as I know, I'm not frooping up with licensing. If I am frooping around with licenses, then ***let me know ASAP!***

### Resource Credits

Sample images (crate, awesome face), used in AGL test, by [Learn OpenGL](http://www.learnopengl.com), licensed under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/).
