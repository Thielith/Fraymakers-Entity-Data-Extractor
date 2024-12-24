COMPILER := g++
FLAGS_ERROR := -Wall -Wextra -Werror

SOURCE := extractor.cpp
RESULT := libEntityExtractor.so
OBJ = $(SOURCE:.cpp=.o)
DEPENDACY := $(SOURCE:.cpp=.d)

# execute rules even if files exist
.PHONY: all clean

all: shared

shared: $(OBJ)
	$(COMPILER) -shared $(OBJ) -o $(RESULT)

%.o: %.cpp
	$(COMPILER) $(FLAGS_ERROR) -MMD -MP -c -fPIC $< -o $@

clean:
	$(RM) $(OBJ) $(DEPENDACY) $(RESULT)


