TARGET = json_eval

CXX = g++

CXXFLAGS = -Wall -g -std=c++11

SRCS = json_eval.cpp helper.cpp
OBJS = $(SRCS:.cpp=.o)


all: build

build: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

run: build
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all build run clean
