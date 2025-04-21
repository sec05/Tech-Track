# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Directories
SRC_DIR = .
UTILS_DIR = ../Prediction/LSTM
INCLUDE_DIRS = -I$(SRC_DIR) -I$(UTILS_DIR)

# Libraries
LIBS = -lgtest -lgtest_main -larmadillo -lpthread

# Files
TARGET = LSTMUtils_test
TEST_SRC = $(SRC_DIR)/$(TARGET).cpp
UTILS_SRC = $(UTILS_DIR)/Utils.cpp
OBJS = $(TEST_SRC:.cpp=.o) $(UTILS_SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Link the test binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -Wl,-rpath,/opt/homebrew/lib -o $@ $^ $(LIBS)

# Compile each cpp file to object
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Run tests
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET) $(OBJS)
