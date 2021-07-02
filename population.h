#ifndef POPULATION_H_INCLUDED
#define POPULATION_H_INCLUDED

#include "macros.h"
#include "structures.h"
#include "crud_gene.h"
#include "evaluation.h"
#include "first_fit.h"

void createPop();
void generate_random_containers(geneptr t1);
void elitism(popptr h1);
void emptyPopulation(popptr h1);

#endif // POPULATION_H_INCLUDED
