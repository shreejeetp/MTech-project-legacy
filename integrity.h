#ifndef INTEGRITY_H_INCLUDED
#define INTEGRITY_H_INCLUDED

#include "macros.h"
#include "structures.h"

int sizeIntegrityCheck(float no);
void integrity_all_containers(geneptr t1);
void countContainers(geneptr t1);
int findAbsentContainers(geneptr t1,int cn_absent[]);
void vmSizeIntegrityCheck(vmptr vm);
void pmSizeIntegrityCheck(pmptr pm);

#endif // INTEGRITY_H_INCLUDED
