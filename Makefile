.PHONY : build test run testrun br tr

OBJS = src/*.cpp

CC = g++

OBJ_NAME = bin/Debug/ECI
TEST_NAME = bin/Debug/test

build:
	$(CC) -static-libstdc++ $(OBJS) -o $(OBJ_NAME)

test:
	$(CC) -static-libstdc++ $(OBJS) -o $(TEST_NAME)

run:
	$(OBJ_NAME).exe

testrun:
	$(TEST_NAME).exe

br: build run

tr: test testrun