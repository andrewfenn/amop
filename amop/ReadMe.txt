========================================================================
                       MockObject++ Overview
========================================================================

AMOP 0.3 is an Automatic Mock Object for C++.

By using ABI and template techniques, it can simulate a pseudo-"Reflection" 
which is normally not support in C++ .

The main differences between AMOP and other mock object library is that, 
users DO NOT need to implement the interface of the object which to mock... 




AMOP uses the CMake system to generate makefiles or if you develop
using and IDE, CMake can make also generate project files for Eclipse
or Microsoft Development Studio. Please see cmake.org for details
about CMake.

========================================================================
Getting Started With GCC
========================================================================
- Get the Sources (lets say into /amop/trunk

- Optionally get and install the package for UnitTest++

- Create a new directory (e.g. /amop/trunk/build)

- Go into the directory created above (e.g. /amp/trunk/build)

- Type the command     cmake -i ../amop      to generate makefiles
  Note: The directory ../amop will have a file called CMakeLists.txt
  Note: The normal way is to have one directory tree for the sources
        and a second, parallel directory tree for the binaries.
  
- Answer the questions. Normally just hit enter to accept the default
  values. The hard question will be for the directory path to the unit 
  test framework. We used UnitTest++ to test that AMOP functions as
  we intended.
  You can omit this answer and the only problem will be that the 
  unit tests OF the AMOP framework will NOT be built. AMOP itself
  will build just fine.
  
- Type the command    make     to make the binaries

Look at the file source/Test/TestMockObject.cpp for examples of the Amop API



========================================================================
Getting Started With Microsoft Dev Studio
========================================================================
- Download and install CMake from cmake.org (we developed using verion 2.6)

- Get the sources (lets say into c:\amop\trunk)

- Create a build directory (lets say c:\amop\trunk\build)

- Run CMake
  Use the source directory (e.g. c:\amop\trunk\amop) for the first entry
  in the data box. Use the build directory as the second directory.
  
  Note: The normal way is to have one directory tree for the sources
        and a second, parallel directory tree for the binaries.
  
  
  - Press the Configure Button
  
  - Select the type of IDE you have from the drop-down, press OK, and then wait
  
  - Enter in a directory for UNITTEST_INCLUDE_DIR. If you do not use UnitTest++
    or do not want to have it listed, just enter in some valid path. This
    directory is only used for building the UnitTest of AMOP. It is not needed
    to be able to use AMOP.
  
- Open the project (e.g. c:\amop\trunk\build\amop.sln), and then build the binaries.

Look at the file source/Test/TestMockObject.cpp for examples of the Amop API
