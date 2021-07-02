#ifndef CRUD_VM_H_INCLUDED
#define CRUD_VM_H_INCLUDED

#include "macros.h"
#include "structures.h"
#include "integrity.h"
#include "evaluation.h"

vmptr createVm(int vmi);
cnptr createCn(int cni);
int selectRandomVmForContainer(cnptr cn);
int vmHasSpaceForContainer(vmptr vm,cnptr cn);
void addCnToVm(cnptr cn,vmptr vm);
int pmHasSpaceForVm(pmptr pm,vmptr vm);
int pmHasSpaceForContainer(pmptr pm,cnptr cn);
pmptr createPm();
void addVmToPm(vmptr vm,pmptr pm);
void sortPmByUtlz(geneptr t1,int sort_by_cpu);
void swapPm(geneptr t1,int x,int y);
void removeContainer(int cni,geneptr t1);
void swapContainers(vmptr vm,int x,int y);
void sortContainersByRes(geneptr t1);
vmptr findVmForCn(geneptr t1,cnptr cn);
pmptr findPmForVm(geneptr t1,vmptr vm);
pmptr findPmByVm(vmptr vm,geneptr t1);
void addCnToVmAndPm(cnptr cn, vmptr vm, pmptr pm);
pmptr createCopyPm(pmptr pm);
void removePm(pmptr pm1,geneptr t1);
void removeVm(vmptr vm1, pmptr pm);
void sortVmbyRes(geneptr t1);
void swapVm(pmptr pm, int x, int y);
int can2VmsBeReplacedBy1(vmptr first_vm, vmptr second_vm, pmptr pm);
int canThisVmBeReplacedByLargerVm(vmptr vm,pmptr pm);
void replace2VmsByRandom(vmptr first_vm,vmptr second_vm,pmptr pm);
void replaceThisVmByLargerVm(vmptr vm,pmptr pm);
vmptr createCopyVm(vmptr vm);
void removeEmptyVmAndPms(geneptr t1);

#endif // CRUD_VM_H_INCLUDED
