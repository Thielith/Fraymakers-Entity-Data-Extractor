COMPILER := g++
COMPILER_WINDOWS := x86_64-w64-mingw32-g++

FLAGS := -L/usr/local/lib -I/usr/local/include
FLAGS_ERROR := -Wall -Wextra -Werror
INCLUDE_JSON := -ljsoncpp
INCLUDE_JSON_WINDOWS := -ljsoncpp_static

SOURCE := extractor.cpp
RESULT_SHARED := libEntityDataExtractor.so
RESULT_STATIC := libEntityDataExtractor.a
RESULT_SHARED_WINDOWS := libEntityDataExtractorWindows.so
RESULT_STATIC_WINDOWS := libEntityDataExtractorWindows.a
OBJ = $(SOURCE:.cpp=.o)
DEPENDACY := $(SOURCE:.cpp=.d)

# execute rules even if files exist
.PHONY: all clean

all: static shared

shared: $(OBJ)
	$(COMPILER) -shared $(OBJ) $(INCLUDE_JSON) -o $(RESULT_SHARED)

static: $(OBJ)
	ar rcs $(RESULT_STATIC) $(OBJ) $(INCLUDE_JSON)

%.o: %.cpp
	$(COMPILER) -static $(FLAGS_ERROR) -MMD -MP -fPIC -c $< -o $@

windows:
	$(MAKE) COMPILER=$(COMPILER_WINDOWS) INCLUDE_JSON=$(INCLUDE_JSON_WINDOWS) RESULT_SHARED=$(RESULT_SHARED_WINDOWS) RESULT_STATIC=$(RESULT_STATIC_WINDOWS) all

clean:
	$(RM) $(OBJ) $(DEPENDACY)


