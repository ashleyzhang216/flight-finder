CPP      = g++
CPPFLAGS = -std=c++20 -Wall
OPTFLAGS = -O3
LIBS     = -lm

SRC_DIR  = ./src
TEST_DIR = ./test

# Binaries to build
NAIVE_BIN    = naive.x
SERIAL_BIN   = serial.x
PARALLEL_BIN = parallel.x
TEST_BIN     = test.x

# Source files shared by all programs
COMMON_SRCS = $(SRC_DIR)/common_data_types.cpp $(SRC_DIR)/parser.cpp
COMMON_OBJS = $(COMMON_SRCS:%.cpp=%.o)

# Individual program sources
NAIVE_SRC = $(SRC_DIR)/naive.cpp
SERIAL_SRC = $(SRC_DIR)/serial.cpp
PARALLEL_SRC = $(SRC_DIR)/parallel.cpp
TEST_SRC = $(TEST_DIR)/test_driver.cpp

NAIVE_OBJ = $(NAIVE_SRC:%.cpp=%.o)
SERIAL_OBJ = $(SERIAL_SRC:%.cpp=%.o)
PARALLEL_OBJ = $(PARALLEL_SRC:%.cpp=%.o)
TEST_OBJ = $(TEST_SRC:%.cpp=%.o)

# Default target builds all
all: $(NAIVE_BIN) $(SERIAL_BIN) $(PARALLEL_BIN) $(TEST_BIN)

naive: $(NAIVE_BIN)
serial: $(SERIAL_BIN)
parallel: $(PARALLEL_BIN)
test: $(TEST_BIN)

# Link rules
$(NAIVE_BIN): $(NAIVE_OBJ) $(COMMON_OBJS)
	$(CPP) $(CPPFLAGS) $(OPTFLAGS) $^ -o $@ $(LIBS)

$(SERIAL_BIN): $(SERIAL_OBJ) $(COMMON_OBJS)
	$(CPP) $(CPPFLAGS) $(OPTFLAGS) $^ -o $@ $(LIBS)

$(PARALLEL_BIN): $(PARALLEL_OBJ) $(COMMON_OBJS)
	$(CPP) $(CPPFLAGS) $(OPTFLAGS) $^ -o $@ $(LIBS)

$(TEST_BIN): $(TEST_OBJ) $(COMMON_OBJS)
	$(CPP) $(CPPFLAGS) $(OPTFLAGS) $^ -o $@ $(LIBS)

# Compile .cpp to .o
%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(OPTFLAGS) -c $< -o $@

clean:
	rm -f *~ *.o
	rm -f $(SRC_DIR)/*~ $(SRC_DIR)/*.o
	rm -f $(TEST_DIR)/*~ $(TEST_DIR)/*.o

realclean: clean
	rm -f *.x