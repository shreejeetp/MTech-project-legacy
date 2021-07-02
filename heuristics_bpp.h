#ifndef HEURISTICS_BPP_H_INCLUDED
#define HEURISTICS_BPP_H_INCLUDED

#include "macros.h"
#include "structures.h"
#include "crud_gene.h"
#include "crud_vm.h"
#include "integrity.h"
#include "first_fit.h"

void heuristics2(geneptr t1,int cn_absent[],int absent_count);
void ff_rc_heuristics(cnptr cn,geneptr t1);
void allocate_list_ff_rc(geneptr t1,int cn_list[],int list_length);
vmptr findVmForCnCompleteFit(geneptr t1,cnptr cn);
vmptr findVmForCnDblBttrFit(geneptr t1,cnptr cn);
vmptr dblReplacementIsPossible(cnptr cn_to_be_replaced,int dbl_rep_cn_list[],geneptr t1);
vmptr singleReplacementIsPossible(cnptr cn_to_be_replaced,int single_rep_cn_list[],geneptr t1);

#endif // HEURISTICS_BPP_H_INCLUDED
