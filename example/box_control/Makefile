
CXX = g++
CFLAGS = -Wall -Wextra -Wunreachable-code -Os
CPPFLAGS=
LDFLAGS=
LDLIBS=

TARGET = main
OBJS = main.o rs485.o motorcord.o

INC_PATH = -I rs485
LIB_PATH = -L rs485 -l rs485

.PHONY: run
run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	rm *.bin *.a *.o

linux.bin: main.cpp sensorimotor.hpp sensorimotor.a
	g++ -o linux.bin main.cpp sensorimotor.a

sensorimotor.o: sensorimotor.cpp
	g++ -c sensorimotor.cpp -o sensorimotor.o

sensorimotor.a: sensorimotor.o
	ar rvs sensorimotor.a sensorimotor.o

