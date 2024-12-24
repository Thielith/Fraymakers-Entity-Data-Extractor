# Fraymakers Entity Data Extractor
 A "library" that extracts the data of a Fraymakers .entity file into a easy-to-use struct.

## Usage
I don't know how to make a thing that installs dynamic/shared libraries yet so just download the repository and put it in your project (while following the license of course).

Once installed, create a pointer of the `entityDataExtractor` class and call `extractEntityData(filePath)`, where `filePath` is a `std::string` representing the path to the .entity file (including the file).

Example: `"project/entities/file.entity"`

## Libraries Used
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp): For parsing the .entity files

## Compiling (Devs)
### Linux
1. Download source files.
2. Run the makefile.

### Windows
Setting up the right dependicies on Windows took me a long time so there will be unnecessary steps below if you're an experienced dev.
If these instructions fail, please create an issue about it.

1. Download source files.
2. Go to [MSYS2](https://www.msys2.org/) and follow the install guide to get the Windows C/C++ compilers.

#### Option 1:  VS Code
3. Get [VS Code](https://code.visualstudio.com/)
4. Install the C/C++ extension
5. With `main.cpp` selected, click the run button in the top right (looks like a > play button)

#### Option 2:  CMake
3. Install [CMake](https://cmake.org/)
4. Download [Ninja](https://ninja-build.org/) and put it somewhere safe
5. Add the following enviroment variables (User or System)
    - `CC` which points to `[/path/to/msys64]/ucrt64/bin/gcc.exe`
    - `CXX` which points to `[/path/to/msys64]/ucrt64/bin/g++.exe`
6. Add the following paths to the PATH enviroment variable (User or System)
    - `[/path/to/MSYS2]/ucrt64/bin`
    - The path to the Ninja file you downloaded in 2
7. Open the command prompt in the source files folder and run these two commands
    - `cmake --preset=default`:  loads build preset
    - `cmake --build build`:  makes the .exe
