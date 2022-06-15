# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
#g++ -g ./src/client/client.cpp -o ./dist/client
#g++ -g ./src/server/*.cpp -lboost_system -lboost_thread -lboost_filesystem -o ./dist/server
CC = g++
CFLAGS = -Wall -g
CLINKS = -lboost_system -lboost_thread -lboost_filesystem
CL_SOR = ./src/client/client.cpp
CL_DES = ./dist/client
SER_SOR = ./src/server/*.cpp
SER_DES = ./dist/server

default:
	$(CC) $(CFLAGS) $(CL_SOR) -o $(CL_DES)
	$(CC) $(CFLAGS) $(SER_SOR) $(CLINKS) -o $(SER_DES)
	