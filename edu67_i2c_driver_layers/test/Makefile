# Unit tests on the PC: the same driver code as on the board, with fakes
# instead of hardware. Run from this folder:  make
#
# -std=gnu++11 on purpose: it is the language level of avr-gcc in the
# Arduino AVR core, so whatever compiles here also compiles for the Uno.

CXX      ?= g++
CXXFLAGS := -std=gnu++11 -Wall -Wextra -Werror -g -I.. -I.

PRODUCTION := \
	../I2cDrvBase.cpp \
	../I2cWireDrv.cpp \
	../I2cItDrv.cpp \
	../Ds3231TempDrv.cpp \
	../At24c32Drv.cpp

TESTS := $(wildcard *.cpp)
TARGET := unit_tests

.PHONY: all clean

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(PRODUCTION) $(TESTS) $(wildcard ../*.h) $(wildcard *.h)
	$(CXX) $(CXXFLAGS) $(PRODUCTION) $(TESTS) -o $@

clean:
	rm -f $(TARGET) $(TARGET).exe
