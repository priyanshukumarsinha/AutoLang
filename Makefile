SRC_DIR = .
LEXER_DIR = lexer
PARSER_DIR = parser
BUILD_DIR = build

SRCS = $(SRC_DIR)/main.cpp \
	   $(LEXER_DIR)/lexer.cpp \
	   $(PARSER_DIR)/parser.cpp 

OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/autolangparser

CXX := g++
CXXFLAGS := -I. -Wall -Werror -std=c++17

all: $(TARGET)

# Build Executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $^

# Generic rule to compile any .cpp file into build folder
# The compiler (g++ -c) will automatically parse #include directives in the source and header files.
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)