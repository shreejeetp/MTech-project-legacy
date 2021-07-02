#ifndef FIRST_FIT_H_INCLUDED
#define FIRST_FIT_H_INCLUDED

#include "macros.h"
#include "structures.h"
#include "crud_vm.h"
#include "crud_gene.h"

void first_fit(geneptr t1);
int addVmListToPmListFF(vmptr vm_list[],int vm_list_size,pmptr pm_list[],int pm_list_size);
int addCnListToVmListFF(cnptr cn_list[],int cn_list_size,vmptr vm_list[],int vm_list_size);

#endif // FIRST_FIT_H_INCLUDED
