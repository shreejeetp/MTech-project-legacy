#ifndef CROSSOVER_H_INCLUDED
#define CROSSOVER_H_INCLUDED

#include "macros.h"
#include "structures.h"
#include "crud_gene.h"
#include "crud_vm.h"
#include "integrity.h"
#include "heuristics_bpp.h"
#include "unpack_n_merge.h"

void crossMut(popptr p1);
void crossover(popptr p1);
geneptr naryTrnment(popptr h1);
geneptr crossover2(geneptr parent1,geneptr parent2,int sort_pm_by_cpu);
void postCrossMut(geneptr child,popptr h1);
void mutation(popptr p1);

#endif // CROSSOVER_H_INCLUDED
