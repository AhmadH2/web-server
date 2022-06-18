# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -g
CLINKS = -lboost_system -lboost_thread -lboost_filesystem
CL_SOR = ./src/client/*.cpp
CL_DES = ./dist/client
SER_SOR = ./src/server/*.cpp
SER_DES = ./dist/server

default:
	$(CC) $(CFLAGS) $(CL_SOR) $(CLINKS) -o $(CL_DES)
	$(CC) $(CFLAGS) $(SER_SOR) $(CLINKS) -o $(SER_DES)
	
client:
	$(CC) $(CFLAGS) $(CL_SOR) $(CLINKS) -o $(CL_DES)

server:
	$(CC) $(CFLAGS) $(SER_SOR) $(CLINKS) -o $(SER_DES)