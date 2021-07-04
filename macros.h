#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define NO_POP 10                       //controls the no. of solutions to be created in initial population generation
#define NO_CROSSOVER 100                 //controls the no. of times crossover/mutation will be done.
#define CNT_TYPE cont_syn1500            //controls the containers array to be used from data1 structure.
#define CNT_SIZE_INIT 1500               //mentions the size of the container initially before reduction/changes.
#define CNT_SIZE container_size         //points to a variable that contains the actual size after removing the containers which can't be fit.
#define VM_TYPE vm_syn                  //controls the vm array to be used.
#define VM_SIZE 10                      //mentions the vm array size
#define TRN_SIZE 2                      //determines the size of tournament
#define ELITISM_COUNT 5                 //No of genes preserved after each process of elitism
#define UNPACK_THRSHLD .8               //the threshold below which the pm will be unpacked in crossover/mutation.
#define VM_UNPACK_THRSHLD .9              //the threshold below which the vm will be unpacked in mutation.
#define IDLE_PM_ENERGY_CONSUMPTION 378
//probabilities 10=full, 0= none
#define PROB_CROSSOVER 5

int container_size;

#endif // MACROS_H_INCLUDED
