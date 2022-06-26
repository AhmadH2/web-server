# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation

CXX = g++
CPPFLAGS = -Wall
CPPLINKS = -lboost_system -lboost_thread -lboost_filesystem
CL_SOR = ./src/client/*.cpp
CL_DES = ./dist/client
SER_SOR = ./src/server/*.cpp
SER_DES = ./dist/server

all:
	$(CXX) $(CPPFLAGS) $(CL_SOR) $(CPPLINKS) -o $(CL_DES)
	$(CXX) $(CPPFLAGS) $(SER_SOR) $(CPPLINKS) -o $(SER_DES)
	
client:
	$(CXX) $(CPPFLAGS) $(CL_SOR) $(CPPLINKS) -o $(CL_DES)

server:
	$(CXX) $(CPPFLAGS) $(SER_SOR) $(CPPLINKS) -o $(SER_DES)

clean:
	$(RM) $(SER_DES) $(CL_DES)