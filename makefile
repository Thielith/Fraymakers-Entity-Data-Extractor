COMPILER := g++
FLAGS := -L/usr/local/lib -I/usr/local/include
FLAGS_ERROR := -Wall -Wextra -Werror

SOURCE := extractor.cpp
RESULT_SHARED := libEntityDataExtractor.so
RESULT_STATIC := libEntityDataExtractor.a
OBJ = $(SOURCE:.cpp=.o)
DEPENDACY := $(SOURCE:.cpp=.d)

# execute rules even if files exist
.PHONY: all clean

all: static shared

shared: $(OBJ)
	$(COMPILER) -shared $(OBJ) -o $(RESULT_SHARED)

static: $(OBJ)
	ar rcs $(RESULT_STATIC) $(OBJ)

%.o: %.cpp
	$(COMPILER) $(FLAGS_ERROR) -MMD -MP -fPIC -c $< -o $@

clean:
	$(RM) $(OBJ) $(DEPENDACY)


