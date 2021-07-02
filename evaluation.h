#ifndef EVALUATION_H_INCLUDED
#define EVALUATION_H_INCLUDED

#include "macros.h"
#include "structures.h"

void fitness(geneptr t1);
void printBestGene(popptr h1);
geneptr searchBestGeneUtlz(popptr h1);
void fitness1(geneptr t1);
void calculateContRes(geneptr t1);
void printStats(geneptr t1);
geneptr searchBestGeneUtlzWithMinNpm(popptr h1);
void printBestGeneEnrg(popptr h1);
geneptr searchBestGeneEnrg(popptr h1);
geneptr searchBestGeneEnrgWithMinNpm(popptr h1);

#endif // EVALUATION_H_INCLUDED
