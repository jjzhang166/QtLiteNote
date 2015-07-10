CC = gcc
XX = g++
CFLAGS = -Wall -O –g
TARGET = ./helloworld

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SOURCES))

$(TARGET) : $(OBJS)
	$(XX) $(OBJS) -o $(TARGET)
	chmod a+x $(TARGET)
