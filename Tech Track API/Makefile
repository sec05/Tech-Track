# Makefile for building the TechTrackAPI project
# You must have aramadillo and Boost libraries installed
# https://arma.sourceforge.net/
# https://www.boost.org/
CXX = g++
# Homebrew paths for include and library files
# Adjust these paths if you have a different installation
# or if you are using a different package manager
INCLUDE = -I/opt/homebrew/include
LIBRARY = -L/opt/homebrew/lib
# Adjust the CXXFLAGS and LDFLAGS as per your system's configuration
CXXFLAGS = -std=c++17 $(INCLUDE) -Wall -Wextra -O2 -g
LDFLAGS = $(LIBRARY) -lboost_thread -lboost_json -lssl -lcrypto -lpthread -lboost_system -larmadillo -v

SRC_DIR = .
BUILD_DIR = build
BIN = TechTrackAPI.out
SRCS = $(wildcard Server/*.cpp Handlers/*/*.cpp Models/*/*.cpp  Prediction/*/*.cpp  main.cpp)
OBJS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

$(BIN): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN) $(LDFLAGS)


$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN)

.PHONY: clean
