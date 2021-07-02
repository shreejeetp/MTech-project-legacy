#include "crud_gene.h"

//creates empty gene pointer and returns it.
geneptr create_gene(){
    geneptr t2=(geneptr)malloc(sizeof(struct gene));
    t2->npm=0;
    t2->utlz=0;
    t2->energy_consumption=0;

    //initialize empty pm list
    for(int i=0;i<CNT_SIZE_INIT;i++){
        t2->pm_list[i]=NULL;
    }

    return(t2);

}

//adds the pm list to gene
void addPmListToGene(pmptr pm_list[],int no_of_pms,geneptr t1){
    for(int i=0;i<no_of_pms;i++){
        t1->pm_list[t1->npm]=pm_list[i];
        t1->npm++;
    }
}

void addPmToGene(pmptr pm,geneptr t1){
    t1->pm_list[t1->npm]=pm;
    t1->npm++;
}

//adds gene to population pointer
void addGene(popptr h1,geneptr t1){
    h1->gene_list[h1->n]=t1;
    h1->n++;
}

//removes gene from population
void removeGene(popptr h1,geneptr t1){
    for(int i=0;i<h1->n;i++){
        if(h1->gene_list[i]==t1){
            if(i!=h1->n-1){
                swapGene(h1,i,h1->n-1);
            }
            free(h1->gene_list[h1->n-1]);
            h1->gene_list[h1->n-1]=NULL;
            h1->n--;
            return;
        }
    }
    printf("\nERROR: REMOVE GENE: Cant find the gene passed!");
    exit(0);
}

//swaps positions x and y in gene_list of population pointer_safety
void swapGene(popptr h1,int x, int y){
    if(h1->gene_list[x]==NULL || h1->gene_list[y]==NULL || x >= h1->n || y >= h1->n || x<0 || y<0 ){
        printf("\nERROR: SWAP GENE: Cant find the gene passed!");
        exit(0);
    }
    geneptr t1=h1->gene_list[x];
    h1->gene_list[x] = h1->gene_list[y];
    h1->gene_list[y] = t1;

}

//creates a copy of t1 and returns it.
geneptr createCopyGene(geneptr t1){
    geneptr cpy_gene=create_gene();
    pmptr cpy_pm_list[CNT_SIZE];

    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        cpy_pm_list[i]=createPm();
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            vmptr cpy_vm=createVm(vm->ogi);
            for(int k=0;k<vm->ncn;k++){
                cnptr cn=vm->cn_list[k];
                cnptr cpy_cn=createCn(cn->ogi);
                addCnToVm(cpy_cn,cpy_vm);
            }
            addVmToPm(cpy_vm,cpy_pm_list[i]);
        }
    }
    addPmListToGene(cpy_pm_list,t1->npm,cpy_gene);
    fitness1(cpy_gene);
    return(cpy_gene);
}

int removeDuplicateContainersFromGene(geneptr t1){
    int cn_check[CNT_SIZE];
    for(int i=0;i<CNT_SIZE;i++){
        cn_check[i]=0;
    }
    int duplicates_count=0;
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            for(int k=0;k<vm->ncn;k++){
                cnptr cn=vm->cn_list[k];
                if(cn_check[cn->ogi]==1){
                    //printf("\nIn remve dup: before removeContainer");
                    removeContainer(cn->ogi,t1);
                    k=k-1;
                    //printf("\n\n\nIn remve dup: after removeContainer");
                    duplicates_count++;
                }
                cn_check[cn->ogi]=1;
            }
        }
    }
    return(duplicates_count);
}
