all:  Assign3.o main.o main clean_interm

CLSDEPEND = Assign3.h Plot.h
MAINDEPEND = Assign3.h Plot.h
MAINALL = main.o Assign3.o

CC = g++
DEBUG = -g
STD = c++11
CFLAGS = -Wall -c $(DEBUG) -std=$(STD) 
LFLAGS = -Wall -o 
POSTFLAGS = -lboost_system -lboost_iostreams

main : $(MAINALL)
	$(CC) $(LFLAGS) main $(MAINALL) -std=$(STD) $(POSTFLAGS)

Assign3.o : $(CLSDEPEND)
	$(CC) $(CFLAGS) Assign3.cpp $(POSTFLAGS)

main.o : $(MAINDEPEND)
	$(CC) $(CFLAGS) main.cpp $(POSTFLAGS)

clean_interm:
	\rm *.o

clean: 
	\rm main
