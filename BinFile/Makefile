#
#  untitled
#
#  Created by  on 2011-01-10.
#  Copyright (c) 2011 __MyCompanyName__. 
#  All rights reserved.
#

CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp BinFile.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=BinFile

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean all
clean:
	$(RM) $(EXECUTABLE) $(OBJECTS)