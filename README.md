# Fraymakers Entity Data Extractor
 A "library" that extracts the data of a Fraymakers .entity file into a easy-to-use struct.

## Attributions
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp): For parsing the .entity files

## Installing
I don't know how to upload this stuff to pacman / the windows equivalent so you gotta do it manually.

### Linux
1. Download the files.
2. Place the `.a` library file somewhere safe (I recommend `/usr/local/lib`).
3. Place the `.cpp` and `.h` files somewhere safe (I recommend `/usr/local/include/fraymakers-entity-data-extractor`) 

Note: When compiling your program, be sure to include these places to let the compiler know to look there.
    - Example: `[compiler] project.cpp -I/usr/local/include -o project -L/usr/local/lib`

### Windows (TBD)

## Usage

Once installed, create a variable of type `entityDataExtractor`.  Then, call `extractEntityData(filePath)`, where `filePath` is a `std::string` representing the path to the .entity file (including the file).  The return value of the function is a pointer of type `entityData`.

Example:

    string path = "project/entities/file.entity"
    entityDataExtractor extractor;
	entityData* data = extractor.extractEntityData(path);

Edge cases you'll want to know about when interacting with the struct:

              Entry in Entity            |            Saved Result in Struct
    -------------------------------------+---------------------------------------------
    paletteCollection = null             |  paletteMap->paletteCollectionID = "null"
    paletteMap = null                    |  paletteMap->paletteMapID = "null"
    [symbol object] alpha = null         |  symbols[index]->alpha = -1.0
    [collision box symbol] color = null  |  symbols[collision box index]->color = "null"
    [collision body symbol] color = null |  symbols[collision box index]->color = "null"

### Remeber to link this library and its dependant libraries to your output if you use it!
`-o project -lEntityDataExtractor -ljsoncpp`
###### i learned the hard way that the order of the flags matter

## Compiling the Stuff in the Repository
### Linux
1. Download source files.
2. Run the included makefile.

### Windows (outdated, probably)
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
