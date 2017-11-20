

#include "NeuronNetwork.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>


#define NUMBERLAYERS 10
#define NUMBERNEURONSMAX 25
#define NUMBERLOOP 20
#define NUMBERTHREADDEFAULT 4

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))



// NB : I tested this programm on a Intel Core i7-5500U 2 core 4 threads 
// I had some crash of the programm while using other apps (youtube speacially) at the same time for 15 layers
// Please lower the number of Layers if you think your computer might not handle this 


short int activationFunctionSig(float sum, float threshold);
void loopProcessing(struct Perceptron ** listLayers,int nbthreads,int nbloops);

int main ( int argc, char *argv[] ){

	int max ,nbthreads ,nbloops;
	struct Perceptron ** listLayers = malloc(sizeof(struct Perceptron *) * NUMBERLAYERS);

	double randNumberNeuron[] ={1438, 5886, 1777, 1191, 22793, 1333, 1038, 1049, 1664, 1642, 2362, 1502, 1869, 2005, 2276 };

	// list of the nbthreads that will be use to test the program 
	// I don't have any better observable result past 5 threads because of a problem of clock()
	// but you can add your own if needed 
	int listNbThreads[] = {1,2,3,4,5,10};
	float randThreshold;


	for (int i = 0 ; i < NUMBERLAYERS ; ++i){


		// max is the number of neuron that will be allocated for one perceptron 
		max = randNumberNeuron[i%NUMBERLAYERS];


		// to get the threshold between [0:1]
		randThreshold = (float)rand()/(float)(RAND_MAX);

		listLayers[i] = newPerceptron(NULL,max,NULL);


		if(VERBOSE)
				printf("%d nb neuron %d \n",i,max );

		// we allocate the array of pointer	
		listLayers[i]->neuronList = malloc(sizeof(struct Neuron *)*max);

		// and then the neuron 
		for (int j = 0; j < max; ++j)
			listLayers[i]->neuronList[j] = newNeuron(randThreshold,activationFunctionSig);

		// just for debugging
		listLayers[i]->id = i;
		
		// NB : I didnt use an array of perceptron at First but linked list. Im too lazy to change this part 
		if(i)
			listLayers[i-1]->nextLayer= listLayers[i];

		
			
	}

	// this function iterate through the layers creating connection between a neuron and the ones from the previous layer
	// the weight is given randomly while taking in account the number of neurons (because i would "beat" the threshold all the time if else)
	connectRandom(*listLayers);
	printf("finished init starting process\n");

	// we consider that the first layer is the entry of information 
	// that does mean that the processing function needs to start from the second layer

	listLayers++;

	// we get the potential argument 
	// it has to be an int 

	// by default the best number of thread for my computer is 4 

	if(argc >= 3)
		nbloops = atoi(argv[2]);
	else
		nbloops = NUMBERLOOP;


	if(argc >= 2 && strcmp(argv[1],"-l" ) == 0){
		for (int i = 0; i < NELEMS(listNbThreads); ++i)
			loopProcessing(listLayers,listNbThreads[i],nbloops);

		return 0;
	}
			


	if(argc >= 2)
		nbthreads = atoi(argv[1]);
	else
		nbthreads = NUMBERTHREADDEFAULT;

	loopProcessing(listLayers,nbthreads,nbloops);
	

	listLayers--;
	
	// we free the memory as good gentleman
	freeNetwork(*listLayers);
	return 0;
}


void loopProcessing(struct Perceptron ** listLayers,int nbthreads,int nbloops){

	clock_t start_t, end_t;
	time_t start_Time, end_Time;
	int sumTicks = 0;

	start_Time = time(NULL);

	for (int i = 0; i < nbloops; ++i){

		// tried to monitor the performances with clock but the result didnt check out
		// might be because im using bash on WIN-X, had the same problem on VM of Kali Linux
		// I left it in case it would work on native UNIX system but I dont have one on myself to test

		start_t = clock();
		processLayers(*listLayers,nbthreads);
		end_t = clock(); 

		printf("id %d looop done in %ld clocks \n",i,end_t- start_t);
		sumTicks = sumTicks + end_t- start_t;
		
	}
	end_Time = time(NULL);

	if(nbthreads != 1)
		printf("for %d threads processing's time is %ld or %f \n",nbthreads,end_Time - start_Time , (double) sumTicks/CLOCKS_PER_SEC);
	else
		printf("sequential mode processing's time is %ld or %f \n",end_Time - start_Time , (double) sumTicks/CLOCKS_PER_SEC);


}

short int activationFunctionSig(float sum, float threshold){

	// the  (NUMBERNEURONSMAX/2) was just here to get a more homogeneous result 
	// but im not good in math dont pay too much attention to this 
	if((1/(exp(-sum/(NUMBERNEURONSMAX/2))+1))>threshold)
		return 1;

	return 0;
}