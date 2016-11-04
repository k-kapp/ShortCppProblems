all :  PSO.o PSO_individual.o GA.o GA_individual.o DiffEvo.o DiffEvo_individual.o main.o main clean_interm

MAINALL = main.o PSO.o PSO_individual.o GA.o GA_individual.o DiffEvo.o DiffEvo_individual.o
MAINDEPEND = import_data.h misc.h PSO.h GA.h Plot.h
PSODEPEND = PSO.h misc.h
PSOINDIVDEPEND = PSO.h misc.h
GADEPEND = GA.h misc.h
GAINDIVDEPEND = GA.h misc.h
DIFFDEPEND = DiffEvo.h misc.h
DIFFINDIVDEPEND = DiffEvo.h misc.h

CC = g++
STD = -std=c++11
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) $(STD) 
LFLAGS = -Wall -o
POSTFLAGS = -Wl,--no-as-needed

main : $(MAINALL)
	$(CC) $(LFLAGS) main $(MAINALL) -pthread -lboost_system -lboost_iostreams

main.o : $(MAINDEPEND)
	$(CC) $(CFLAGS) main.cpp -lpthread $(POSTFLAGS) -lboost_system -lboost_iostreams

PSO.o : $(PSODEPEND)
	$(CC) $(CFLAGS) PSO.cpp -lpthread $(POSTFLAGS)

PSO_individual.o : $(PSOINDIVDEPEND)
	$(CC) $(CFLAGS) PSO_individual.cpp -lpthread $(POSTFLAGS)

GA.o : $(GADEPEND)
	$(CC) $(CFLAGS) GA.cpp -lpthread $(POSTFLAGS)

GA_individual.o : $(GAINDIVDEPEND)
	$(CC) $(CFLAGS) GA_individual.cpp -lpthread $(POSTFLAGS)

DiffEvo.o : $(DIFFDEPEND)
	$(CC) $(CFLAGS) DiffEvo.cpp -lpthread $(POSTFLAGS)

DiffEvo_individual.o : $(DIFFINDIVDEPEND)
	$(CC) $(CFLAGS) DiffEvo_individual.cpp -lpthread $(POSTFLAGS)

clean_interm : 
	\rm *.o

clean :
	\rm main
