CPP      = g++
CPPFLAGS = -std=c++20 -Wall
OPTFLAGS = -O3
LIBS     = -lm

# main dirs
SRC_DIR=./src
TEST_DIR=./test

# src dirs
COMMON_DIR=common
CLI_DIR=cli

# test dirs
CATCH_DIR=catch

# binaries
MAIN_BIN = finder.x
TEST_BIN = test_driver.x

SRC_CPPFILES = \
		$(SRC_DIR)/$(CLI_DIR)/finder.cpp

TEST_CPPFILES = \
		$(TEST_DIR)/test_driver.cpp

SRC_HFILES = \
		$(SRC_DIR)/$(CLI_DIR)/finder.h

TEST_HFILES = \
		$(TEST_DIR)/$(CATCH_DIR)/catch.hpp

SRC_OFILES = \
		finder.o

TEST_OFILES = \
		test_driver.o

all: $(MAIN_BIN) $(TEST_BIN)
main: $(MAIN_BIN)
test: $(TEST_BIN)

clean: 
	rm -f *~ *.o

realclean: clean
	rm -f *.x

$(MAIN_BIN) : $(SRC_OFILES)
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LIBS)

$(TEST_BIN) : $(TEST_OFILES)
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LIBS)

$(SRC_OFILES) : $(SRC_CPPFILES) $(SRC_HFILES)
	$(CPP) -c $(CPPFLAGS) $(OPTFLAGS) $< -o $@

$(TEST_OFILES) : $(TEST_CPPFILES) $(TEST_HFILES) $(SRC_CPPFILES) $(SRC_HFILES)
	$(CPP) -c $(CPPFLAGS) $(OPTFLAGS) $< -o $@
