CXX = g++
CFLAGS = -O3 -Wall -std=c++17 -march=native -g
INCLUDES = 
LIBS = -lm

SOURCE_PATH = src
LIB_SOURCE = mtb.cpp mtx.cpp compatibility.cpp
LIB_NAME = libmtb.a

all: converter lib

converter: $(LIB_NAME) converter.o
	$(CXX) $^ -o $@ $(CFLAGS) $(INCLUDES) $(LIBS)
	rm converter.o $(LIB_SOURCE:.cpp=.o)

%.o: $(SOURCE_PATH)/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) $(INCLUDES)

lib: $(LIB_SOURCE:.cpp=.o)
	ar rcs libmtb.a $^
	
clean:
	touch converter $(LIB_NAME)
	rm converter $(LIB_NAME)
