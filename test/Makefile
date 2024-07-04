CXX = g++
CXXFLAGS = -Wall -g
SRC = DriverTest.cpp
OBJ = DriverTest.o
TARGET = test

# Targets
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -f $(TARGET) $(OBJ)

