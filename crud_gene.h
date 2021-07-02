#ifndef CRUD_GENE_H_INCLUDED
#define CRUD_GENE_H_INCLUDED

#include "macros.h"
#include "structures.h"
#include "integrity.h"
#include "crud_vm.h"


geneptr create_gene();
void addPmListToGene(pmptr pm_list[],int no_of_pms,geneptr t1);
void addGene(popptr h1,geneptr t1);
void removeGene(popptr h1,geneptr t1);
void swapGene(popptr h1,int x, int y);
geneptr createCopyGene(geneptr t1);
int removeDuplicateContainersFromGene(geneptr t1);
void addPmToGene(pmptr pm,geneptr t1);

#endif // CRUD_GENE_H_INCLUDED
