.PHONY : build test run testrun br tr

OBJS = src/*.cpp

CC = g++

OBJ_NAME = bin/Debug/GIV
TEST_NAME = bin/Debug/test

build:
	$(CC) -g $(OBJS) -o $(OBJ_NAME)

release:
	$(CC) $(OBJS) -o $(OBJ_NAME)

buildComp:
	$(CC) -g src/compression/*.cpp -o comp

run:
	$(OBJ_NAME).exe

testrun:
	$(TEST_NAME).exe

br: build run

tr: test testrun
