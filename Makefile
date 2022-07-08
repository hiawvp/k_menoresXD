CPP=g++ -std=c++14
CPPFLAGS=-O3 -Wall -DVERBOSE
INCLUDES=-I./include/
SOURCES=./include/BasicCDS.cpp main.cpp

unary:
	@echo " [BLD] Building binary unary"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) $(SOURCES) -o main
