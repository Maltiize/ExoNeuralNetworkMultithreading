//#include "List.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define CHUNK 150
#define VERBOSE 0



struct Connection
{
	struct Neuron * entry;
	float weight;
};


struct Neuron 
{
	short int activationState ;
	float threshold;
	short int (*activationFunction)(float,float);
	struct Connection ** connectionIn;
	unsigned int nbConn;


};


struct Perceptron 
{
	struct Neuron ** neuronList;
	struct Perceptron * nextLayer;
	int sizeNeuronList ;
	int id;
};




struct Connection * newConnection(struct Neuron * entry,float weight);
struct Neuron * newNeuron(float threshold,short int (*activationFunction)(float,float));
struct Perceptron * newPerceptron(struct Neuron **  neuronL, int size,struct Perceptron * next);
void freeNetwork(struct Perceptron * p);
void freePerceptron(struct Perceptron * p );
void freeNeuron(struct Neuron * n);
void processNeuron (struct Neuron * neuron);
void processLayers(struct Perceptron * layer,int nbThreads);
void connectRandom(struct Perceptron * p);
void connectRandomPerceptron(struct Perceptron * first,struct Perceptron *  second,float maxWeight);

