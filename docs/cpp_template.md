# CPP project template for Gitpod and VSCode

A template project for C++ development using [CMake](https://cmake.org/) and [Conan](http://conan.io/) package manager.

## Gitpod

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/mfouesneau/gitpod-cpp-project-template)


Gitpod is a friendly online IDE very similar to VSCode ([Gitpod.io](https://gitpod.io/)
In particular, it provides an entire container-based platform (not saying Docker) and provides VSCode features such as pair-coding while still compiling and running codes. It's also very convenient to write/edit and run some codes rapidly.

This repository holds a very basic workspace to start a C++ project and to work with Gitpod.

ℹ️ **The rest of the README contains some notes on the template and project configuration**

[TOC]

## Example Code

The example codes are basic
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
#include <iostream>
namespace example {
	void example1(){
	    std::cout << "Example 1 called. \n";
	}
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
But also show how to install and use external libraries with `conan`. I give the example of `xtensor` (https://github.com/xtensor-stack/xtensor)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
#include <iostream>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"
#include "example.hpp"

int main() {
    xt::xarray<double> arr1
    {{1.0, 2.0, 3.0},
    {2.0, 5.0, 7.0},
    {2.0, 5.0, 7.0}};

    xt::xarray<double> arr2
    {5.0, 6.0, 7.0};

    xt::xarray<double> res = xt::view(arr1, 1) + arr2;

    std::cout << res;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Conan Package Manager


[Conan](http://conan.io/) is an Open Source package manager for C and C++ development. It allows developers to quickly and efficiently manage their packages and dependencies across platforms and build systems.

In this project, we use Conan to manage our dependencies and build the project with [CMake](https://cmake.org/).

### Installing and Configuring Conan

I use python installation

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-shell}
> pip install conan
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You may have to look for the library you need in the [package center](https://conan.io/center/), e.g.:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-shell}
> conan search xtensor -r conan-center
Existing package recipes:

xtensor/0.21.2
xtensor/0.21.3
xtensor/0.21.4
xtensor/0.21.5
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add your requirements to the `conanfile.txt` in the `requires` section, e.g.,
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-yaml}
[requires]
 xtensor/0.21.5
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 ### Using Conan with CMake

 add `cmake` in the `generator` section to the `conanfile.txt`, e.g,
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-yaml}
[generators]
 cmake
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Update your `CMakeLists.txt` to include the conan libraries, for instance before the targets
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-makefile}
include(conanbuildinfo.cmake)
conan_basic_setup()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
And for the relevant targets
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-makefile}
target_link_libraries(example ${CONAN_LIBS})
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Finally, you will need to run `conan install .` before `cmake`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-shell}
> conan install . && cmake .
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Workspace configuration

Gitpod configuration is very similar to a VSCode workspace. The main difference is that Gitpod is a container-based platform, which means you can define your container images.

### Container image `.gitpod.Dockerfile`

More information: https://www.gitpod.io/docs/config-docker/

By default, Gitpod uses a standard image called `gitpod/workspace-full`.
This default image comes pre-installed with Docker, Go, Java, Node.js, C/C++, Python, Ruby, Rust, PHP, and tools such as Homebrew, Tailscale, Nginx, and several more.

If this image does not include the tools you need for your project, you can provide a public Docker image or your Dockerfile.
Using this technique provides us with the flexibility to install the tools & libraries required for your project.

> This template starts with this image as it will make sure that the platform works (sometimes new Debian images introduce breaking changes; see documentation) and installs the HDF5 library.


### Gitpod configuration `.gitpod.yml`

Here is the minimal configuration I use, which tells Gitpod to
* use the provided image information,
* install `conan` and run the first build on start,
* and install some C/C++ convenient extensions for VScode.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-yaml}
image:
  file: .gitpod.Dockerfile

tasks:
  - init: "pip install conan"
  - command: "conan install . && cmake ."

vscode:
  extensions:
    - webfreak.debug
    - ms-vscode.cmake-tools
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Launchers and Tasks Definitions (`.vscode/tasks.json`, `.vscode/launch.json`)

VScode based IDEs can do the everyday IDE tasks but need some primary learning curve to set things up.

I set up this template to provide a workflow that compiles the code and runs it (with debugging options).
* A task that compiles the main code with `CMake` and `Make`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-json}
{
    "label": "DEBUG: cmake && make",
    "type": "shell",
    "command": "cd ${workspaceRoot} && cmake . -DCMAKE_BUILD_TYPE=Debug && make",
    "problemMatcher": [],
    "group": {
        "kind": "build",
        "isDefault": true
    }
},
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ℹ️  **Note**: `${workspaceRoot}` is the root of the workspace.

:warning: **Note**: `-DCMAKE_BUILD_TYPE=Debug` is essential to make the debugger catch breakpoints etc. The cost is that the build is slower and performance lower. (I also added a task to build the _release_ version)

* A launch configuration that runs the executable with `GDB` after running the task mentioned above.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-json}
{ "type": "gdb",
  "request": "launch",
  "name": "GDB: blackbodystars",
  "target": "${workspaceRoot}/bin/blackbodystars",
  "args": [],
  "cwd": "${workspaceRoot}",
  "valuesFormatting": "parseText",
  "preLaunchTask": "DEBUG: cmake && make"
},
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The `CMakeLists.txt` tells where the compilation puts the `target` executable.

:warning: **Note**: In VScode, the default launcher is with `cppdbg`. The associated extension (`ms-vscode.cpptools`) is not part of the open extension library (open VSX). (will it be?). Still, I also provide the corresponding launcher in the configuration file to use offline or remotely with VSCode.


## Code documentation

**Problem statement**:
It is finally time to set up API documentation for the project. It's better to start at the beginning with good pratice. Some of the requirements are:

* documentation should be generated by a CMake file
* only generated in _Release_ mode (not _Debug_)
* should allow some user documentation files (Markdown) which are not in the source code folder.


I set this up with [Doxygen](https://www.doxygen.nl/index.html).
The official Doxygen site contains plenty of information on using the Doxygen syntax and generating *.html files of documentation.

⚠️ **Note**: Annoyingly, _doxygen_ does not use the GitHub markdown flavor syntax.

### Some incomplete notes for documentation

We document C++ code in two ways:
1. by writing documentation blocks for all public or protected C++ components (namespaces, types, methods, functions, and constants).
2. by commenting our code internally with C++ comments (`//` or `/* .. */`). The latter is only visible in the source.

#### Documentation Blocks
Multi-line documentation blocks must begin with `/**` and end in `*/`.

Single-line documentation blocks usually begin with `///` but are rarely used for public APIs.
Under certain circumstances, single-line documentation blocks may begin with `///<`.

Annotating parameters with inline Comments `///<` is an alternative to the `@param` tag. This style is permitted for historical reasons, but one should avoid it in new codes.


Documentation blocks must use Javadoc-style tags such as `@see` or `@param`

The documentation MUST appear before the declaration it describes and with the same indentation. See example:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{cpp}
/**
 * Sum numbers in a vector.
 *
 * @param values Container whose values are summed.
 * @return sum of `values`, or 0.0 if `values` is empty.
 */
double sum(std::vector<double> & const values) {
    ...
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`@param` should be given with the `[in]`, `[out]`, or `[in, out]` tag if the function method contains any output parameters. The `[in]` tag is optional if all parameters are input, even if other functions or methods in the same class or package use output parameters.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{cpp}
/**
 * Compute mean and standard deviation for a collection of data.
 *
 * @param[out] mean the mean of `data`, or `NaN` if `data` is empty
 * @param[out] stdDev the unbiased (sample) standard deviation, or `NaN`
 *     if `data` contains fewer than 2 elements
 * @param[in] data the data to analyze
 */
void computeStatistics(double & mean, double & stdDev, std::vector<double> const & data);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Unittests

If you are not familiar with the concept of unit testing, the basic principle is to write small pieces of code that tests every new piece of functionality in your project.

It is very useful to have such unit tests, because any development or changes in your code can be tested against expected results. If something does not match, you will know rapidly.

I decided to provide a template for one's own testing frameworl and CMake:
* Use [`CTest`](https://cmake.org/cmake/help/latest/manual/ctest.1.html), the testing framework of CMake. This framework is rather simple and just uses the return type of a unit test program to decide whether the test worked correctly.
* Provide a set of routines to check the correctness of certain calculations within a unit test, throwing an error if something unexpected happened.
* Add the compilations of the test codes to `CMakeLists.txt`.

In practice, the test fails when throwing an error (the unit test program crashed).

First one need to update `CMakeLists.txt` to include the testing library.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-cmake}
ENABLE_TESTING()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Adding a test becomes creating a source code into the `tests` folder and updating the `CMakeLists.txt` for something like
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-cmake}
ADD_EXECUTABLE( test_xtensor test_xtensor.cpp)
TARGET_LINK_LIBRARIES(test_xtensor
        ${blas_libraries} ${lapack_libraries}
        ${CONAN_LIBS})
ADD_TEST(io_xtensor test_xtensor)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

By setting `ENABLE_TESTING()`, CMake actually creates a new target called `test`. Hence, to run those tests, `make test` from the build directory is sufficient.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~{lang-shell}
Running tests...
Test project /workspace/gitpod-cpp-project-template
    Start 1: example_tests
1/1 Test #1: example_tests ....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.00 sec
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> this unit test can then be part of a Travis CI or GitHub actions workflow.

GitHub Actions CI/CD
--------------------

This repository sets already some default actions to help the project development (see `.github/workflows/workflow.yml`)

 - The `Build` job will take the current CMake configuration and run the build in `Release` mode and the unittests (`ctest`)
 - The `Documentation` job, uses the preconfigured Doxygen settings to generate the HTML API documentation. (One can extend this job to pubish on `gh-pages` branch)