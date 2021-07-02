#include "integrity.h"


int sizeIntegrityCheck(float no){
    if(no<0){
        if(no>-0.01){
            return(0);
        }
        return(-1);
    }
    return(1);
}

//checks if all containers are present in the gene and if any duplicates are present
void integrity_all_containers(geneptr t1){
    int cn_check[CNT_SIZE];
    for(int i=0;i<CNT_SIZE;i++){
        cn_check[i]=0;
    }

    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            for(int k=0;k<vm->ncn;k++){
                cnptr cn=vm->cn_list[k];
                if(cn_check[cn->ogi]==1){
                    printf("\nERROR: In Integrity all containers: Duplicates");
                    exit(0);
                }
                cn_check[cn->ogi]=1;
            }
        }
    }
    for(int i=0;i<CNT_SIZE;i++){
        if(cn_check[i]==0){
            printf("\nERROR: In Integrity all containers: Container Absent:%d",i);
            exit(0);
        }
    }
    printf("\nIntegrity Check passed: All containers present!");
}

//counts containers in t1 and prints the no.
void countContainers(geneptr t1){
    int count=0;
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            count+=vm->ncn;
        }
    }
    printf("\nno of containers:%d",count);
}

//fills the cn_absent with the containers which are absent, and returns the absent count
int findAbsentContainers(geneptr t1,int cn_absent[]){
    int cn_check[CNT_SIZE];
    for(int i=0;i<CNT_SIZE;i++){
        cn_check[i]=0;
    }

    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            for(int k=0;k<vm->ncn;k++){
                cnptr cn=vm->cn_list[k];
                if(cn_check[cn->ogi]==1){
                    printf("\nERROR: In Integrity all containers: Duplicates");
                    exit(0);
                }
                cn_check[cn->ogi]=1;
            }
        }
    }
    int absent_count=0;
    for(int i=0;i<CNT_SIZE;i++){
        if(cn_check[i]==0){
            cn_absent[absent_count]=i;
            absent_count++;
        }
    }
    return(absent_count);
}
void vmSizeIntegrityCheck(vmptr vm){
    int integrity_check_cpu=sizeIntegrityCheck(vm->cpu);
    if(integrity_check_cpu==0){
        vm->cpu=0;
    }
    else if(integrity_check_cpu==-1){
        printf("\nERROR: Vm cpu size integrity check failed!:%f",vm->cpu);
        exit(0);
    }

    int integrity_check_mem=sizeIntegrityCheck(vm->mem);
    if(integrity_check_mem==0){
        vm->mem=0;
    }
    else if(integrity_check_mem==-1){
        printf("\nERROR: Vm mem size integrity check failed!%f",vm->mem);
        exit(0);
    }
}

void pmSizeIntegrityCheck(pmptr pm){
    int integrity_check_cpu = sizeIntegrityCheck(pm->cpu);
    if(integrity_check_cpu==0){
        pm->cpu=0;
    }
    else if(integrity_check_cpu==-1){
        printf("\nERROR: pm cpu size integrity check failed!%f",pm->cpu);
        exit(0);
    }

    int integrity_check_mem = sizeIntegrityCheck(pm->mem);
    if(integrity_check_mem==0){
        pm->mem=0;
    }
    else if(integrity_check_mem==-1){
        printf("\nERROR: pm mem size integrity check failed!%f",pm->mem);
        exit(0);
    }
}
