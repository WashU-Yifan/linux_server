CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LDFLAGS = -lpthread -lrt
SOURCES = pool/main.cpp pool/tpool.cpp 
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = Webserver

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)