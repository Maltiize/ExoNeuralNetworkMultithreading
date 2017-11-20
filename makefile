ccflags-yOpen := -Wall -std=c99 -lm -fopenmp -o 

programme : parallel.o 
 
parallel.o : 
	gcc  main.c NeuronNetwork.c $(ccflags-yOpen) parallel

clean : 
	rm ./parallel

