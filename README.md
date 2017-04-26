# polybobin
Map maker for Soldat.

The project is written in C++ and depends on:<br />
- [wxWidgets 3.1.0](https://github.com/wxWidgets/wxWidgets)<br />
- OpenGL 3.3<br />
- [GLM](https://github.com/g-truc/glm)<br />

Tests depend on:<br />
- [GTest and GMock](https://github.com/google/googletest)

Finally, documentation relies on:<br />
- [Doxygen](https://github.com/doxygen/doxygen)<br />

### Building on GNU/Linux
Run this from root directory to compile:<br />
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```
The executable file is in polybobin's bin/ directory.<br />
<br />
To run tests, you can:<br />
- run `make test` from root directory to run them all at once.<br />
- run binaries in build/test/ directory individually.

### Building on Windows (Visual Studio)
First of all, make sure you have [CMake](https://cmake.org/) installed. Open Command Prompt and navigate to root directory. Then run this:<br />
```
mkdir build
cd build
cmake ..
```
CMake might complain about missing libraries. This can be solved by downloading and building the necessary libraries and listing their paths in environment variables.<br />
CMake expects to find libraries by looking at paths provided in these environment variables:<br />
`GLM_ROOT_DIR` -> path of GLM's root directory.<br />
`GMOCK_ROOT` -> path of GMock's root directory AND path of GMock's .lib file after it's compiled (paths separated by semicolon).<br />
`GTEST_ROOT` -> path of GTest's root directory AND path of GTest's .lib file after it's compiled (paths separated by semicolon).<br />
`WXWIN` -> path of wxWidgets' root directory.<br />

Now open the polybobin solution in build/ directory and build it.<br />
Building "INSTALL" project will move polybobin's executable file to bin/ directory.<br />
Building "RUN_TESTS" project will run tests.<br />

### Documentation
To generate documentation, run `doxygen doxygen.config`.
