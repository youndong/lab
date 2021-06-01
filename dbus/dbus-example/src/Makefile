CC		= g++
INCLUDE	= -I/usr/include/dbus-c++-1/ -I$(CURDIR)/../include
CFLAGS	= -g -Wall 
SERVICE	= service
CLIENT	= client
LIBS	= -ldbus-c++-1 -lpthread
BIN_DIR = $(CURDIR)/bin

all:
	rm -rf $(BIN_DIR); mkdir $(BIN_DIR)
	$(CC) $(INCLUDE) server.cpp $(LIBS) -o $(SERVICE) 
	$(CC) $(INCLUDE) client.cpp $(LIBS) -o $(CLIENT) 
	
	mv $(SERVICE) $(BIN_DIR) 
	mv $(CLIENT) $(BIN_DIR) 

clean :
	rm -rf $(BIN_DIR)/* 

