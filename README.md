

# Simplistic network for parallel programming's assignment


## To install

```
    make
```

## The command lines

There is no script but everything can be tested with only the command line

```
  ./parallel -l
```

Test the program for 10 loops of the neural network

Will do that sequentially then while using 2,4,5,10 threads one setup after another

you can edit the  ``` 	int listNbThreads[] = {1,2,3,4,5,10}; ``` main.c line 39 to change the list

here are the others options

argument one : the number of thread to use

argument two : the number of propagation's loop inside the network


ex


```
  ./parallel 4 5
```


will do 5 loops using 4 threads
