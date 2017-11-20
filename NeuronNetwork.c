

#include "NeuronNetwork.h"


// allocate memory for a new Connection
struct Connection * newConnection(struct Neuron * entry,float weight){

	struct Connection * tmp = malloc(sizeof(struct Connection));
	tmp->entry = entry;
	tmp->weight = weight;

	return tmp;
}

// allocate memory for a new Neuron 
struct Neuron * newNeuron(float threshold,short int (*activationFunction)(float,float)){

	struct Neuron * tmp = malloc(sizeof(struct Neuron));
	tmp->threshold = threshold;
	tmp->activationFunction = activationFunction;
	tmp->activationState = 0;
	tmp->connectionIn = NULL;


	// This part is only used for the first layer 
	// wich is considered as the entry of informations
	int r = rand()%100;

	if(r>50)
		tmp->activationState = 1;
	else	
		tmp->activationState = 0;


	return tmp;
}

void freeNeuron(struct Neuron * n){


	for (int i = 0; i < n->nbConn; ++i)
		free(n->connectionIn[i]);

	free(n);
}

void freePerceptron(struct Perceptron * p ){

	for (int i = 0; i < p->sizeNeuronList; ++i)
		free(p->neuronList[i]);

	free(p);
		
}

void freeNetwork(struct Perceptron * p){

	struct Perceptron * tmp = p;
	struct Perceptron * next = p->nextLayer;


	do{

		freePerceptron(tmp);
		tmp = next;
		next = next->nextLayer;

	}while(next);
}

// allocate memory for a new newPerceptron
struct Perceptron * newPerceptron(struct Neuron **  neuronL, int size,struct Perceptron * next){

	struct Perceptron * tmp =  malloc(sizeof(struct Perceptron));
	tmp->neuronList = neuronL;
	tmp->sizeNeuronList = size;
	tmp->nextLayer =next;

	return tmp;
}


// Connects all the neurons from each layers to the ones from the previous layer
// the weight for each connection is set randomly
void connectRandom(struct Perceptron * p){

	if(p ==  NULL || p->nextLayer ==  NULL)
		return ;

	struct Perceptron * currentP =  p;
	struct Perceptron * nextP = p->nextLayer;

	while(nextP){

		connectRandomPerceptron(currentP,nextP,(float) nextP->sizeNeuronList);
		nextP = nextP->nextLayer;
		currentP =  currentP->nextLayer;
	}


}


void connectRandomPerceptron(struct Perceptron * first,struct Perceptron *  second,float maxWeight){

	

	struct Neuron *  tmpNeuron ;

	for (int i = 0; i < second->sizeNeuronList; ++i){

		tmpNeuron = second->neuronList[i] ;
		tmpNeuron->connectionIn = malloc(sizeof(struct Connection)*first->sizeNeuronList); 
		tmpNeuron->nbConn = first->sizeNeuronList;
		
		for (int j = 0; j < first->sizeNeuronList; ++j)
			tmpNeuron->connectionIn[j] =  newConnection(first->neuronList[j],((float)(rand()%(int)maxWeight)/maxWeight));

	}
}

// Process every layers with nbThreads threads
void processLayers(struct Perceptron * layer,int nbThreads){

	struct Perceptron * tmpLayer = layer;

	int parallel;

	if(nbThreads <= 1)
		parallel = 0;
	else
		parallel = 1;

	while(tmpLayer){

		if(VERBOSE)
				printf("\n\nlayer %d numberNeur %d Nb connection%d\n\n" ,tmpLayer->id,tmpLayer->sizeNeuronList,tmpLayer->neuronList[0]->nbConn);
		
		#pragma omp parallel  num_threads(nbThreads) if(parallel)
		{
			// dynamic is used without chunk because the number of neuron is each layer can change drastically 
			// but the bug on clock() made it sometimes impossible to see the difference with the others option
			#pragma omp for schedule(dynamic)
			for (int i = 0; i < tmpLayer->sizeNeuronList; ++i)
				processNeuron(tmpLayer->neuronList[i]);
		}

		tmpLayer = tmpLayer->nextLayer;
	}
	
	
}



void processNeuron (struct Neuron * neuron){

	//int i,j;
	float sum = 0;

	struct Connection * tmpConnection;

	if(VERBOSE)
		printf("gonna process %d connection \n",neuron->nbConn );


	// tested to parallelize this loop but as excepted the result were even worse than without using openMP
	/*#pragma omp parallel  num_threads(nbThreads)
	{*/
		//#pragma omp for schedule(dynamic)
		for (int x = 0; x < neuron->nbConn ; ++x){

			tmpConnection = neuron->connectionIn[x];

			if(tmpConnection->entry == NULL)
				continue ;

			sum = sum +  tmpConnection->entry->activationState * tmpConnection->weight;

			if(VERBOSE)
				printf(" %d sum%f activationState %d weight %f \n",x,sum,tmpConnection->entry->activationState , tmpConnection->weight );
		}

	//}

	neuron->activationState = neuron->activationFunction(sum,neuron->threshold);

	if(VERBOSE)
		printf("%f sum  %d activationState\n",sum,neuron->activationState );

}

