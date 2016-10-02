# polybobin
Map maker for Soldat.

The project uses:<br />
1. wxWidgets 3.1.0<br />
2. OpenGL 3.3<br />
3. GLM<br />
4. Doxygen<br />

To compile, run `make`. The executable file is in bin/ directory.
To generate documentation, run `doxygen doxygen.config`.

As of 02/10/2016, it is only a map parser with ability to select polygons and scenery.
Before compiling, adjust PATH_SOLDAT in constants.hpp.
