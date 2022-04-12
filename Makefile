CXX = g++
CFLAGS = -O3 -Wall -std=c++17 -march=native -g
INCLUDES = 
LIBS = -lm

SOURCE_PATH = src
LIB_SOURCE = mtb.cpp mtx.cpp compatibility.cpp
LIB_NAME = libmtb.a

all: lib converter

converter: converter.o $(LIB_NAME)
	$(CXX) $^ -o $@ $(CFLAGS) $(INCLUDES) $(LIBS) -L. -lmtb 
	rm converter.o $(LIB_SOURCE:.cpp=.o)

$(LIB_NAME): lib

lib: $(LIB_SOURCE:.cpp=.o)
	ar rcs $(LIB_NAME) $^

%.o: $(SOURCE_PATH)/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) $(INCLUDES)

clean:
	touch converter $(LIB_NAME)
	rm converter $(LIB_NAME)
