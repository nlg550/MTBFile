CXX = g++
CFLAGS = -O3 -Wall -std=c++17 -g -march=native
INCLUDES = 
LIBS = -lm

SOURCE_PATH = src
SOURCE = converter.cpp mtb.cpp mtx.cpp compatibility.cpp
TARGET = converter

all: $(TARGET)

$(TARGET): $(SOURCE:.cpp=.o)
	$(CXX) $^ -o $@ $(CFLAGS) $(INCLUDES) $(LIBS)

%.o: $(SOURCE_PATH)/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) $(INCLUDES)
	
clean:
	rm $(SOURCE:.cpp=.o) $(TARGET)