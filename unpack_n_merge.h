#ifndef UNPACK_N_MERGE_H_INCLUDED
#define UNPACK_N_MERGE_H_INCLUDED

#include "macros.h"
#include "structures.h"
#include "crud_gene.h"
#include "crud_vm.h"
#include "integrity.h"
#include "heuristics_bpp.h"

void unpackPm(geneptr t1);
float calculateProbOfUnpack(pmptr pm,geneptr t1);
void mergeVm(geneptr t1);


#endif // UNPACK_N_MERGE_H_INCLUDED
