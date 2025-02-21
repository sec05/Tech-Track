CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include -Wall -Wextra -O2
LDFLAGS = -L/opt/homebrew/lib -lboost_system -lboost_thread -lpthread -larmadillo -v

SRC_DIR = .
BUILD_DIR = build
BIN = TechTrackAPI.out
SRCS = $(wildcard Server/*.cpp Handlers/*/*.cpp main.cpp)
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
