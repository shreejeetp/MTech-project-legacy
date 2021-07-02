#include "evaluation.h"


//evaluates,updates and displays the fitness of the passed gene.
void fitness(geneptr t1){
    t1->energy_consumption=0;
    t1->utlz=0;

    printf("\n\nCalculating fitness...");

    for(int i=0; i < t1->npm ; i++){
        pmptr pm=t1->pm_list[i];
        pm->utlz=(pm->totcpu-pm->cpu)/(pm->totcpu);
        pm->mem_utlz=(pm->totmem-pm->mem)/(pm->totmem);
        pm->energy_consumption=(IDLE_PM_ENERGY_CONSUMPTION)+((pm->enrg - IDLE_PM_ENERGY_CONSUMPTION)*pm->utlz);
        for(int j=0; j < pm->nvm; j++){
            vmptr vm=pm->vm_list[j];
            vm->utlz=(vm->totcpu-vm->cpu)/vm->totcpu;
            vm->mem_utlz=(vm->totmem-vm->mem)/vm->totmem;
        }
        printf("\n\tPM:%d\tCpu Utlz:%f\t Mem Utlz:%f",i,pm->utlz,pm->mem_utlz);
        t1->energy_consumption+=pm->energy_consumption;
        t1->utlz+=pm->utlz;
    }
    t1->utlz=t1->utlz/t1->npm;
    printf("\n\nGene:\tEnergy consumption:%f\tUtlz:%f",t1->energy_consumption,t1->utlz);
}

//same as fitness, but doesn't print anything.
void fitness1(geneptr t1){
    t1->energy_consumption=0;
    t1->utlz=0;

    //printf("\n\nCalculating fitness...");

    for(int i=0; i < t1->npm ; i++){
        pmptr pm=t1->pm_list[i];
        pm->utlz=(pm->totcpu-pm->cpu)/(pm->totcpu);
        pm->mem_utlz=(pm->totmem-pm->mem)/(pm->totmem);
        pm->energy_consumption=(IDLE_PM_ENERGY_CONSUMPTION)+((pm->enrg - IDLE_PM_ENERGY_CONSUMPTION)*pm->utlz);
        for(int j=0; j < pm->nvm; j++){
            vmptr vm=pm->vm_list[j];
            vm->utlz=(vm->totcpu-vm->cpu)/vm->totcpu;
            vm->mem_utlz=(vm->totmem-vm->mem)/vm->totmem;
        }
        //printf("\n\tPM:%d\tCpu Utlz:%f\t Mem Utlz:%f",i,pm->utlz,pm->mem_utlz);
        t1->energy_consumption+=pm->energy_consumption;
        t1->utlz+=pm->utlz;
    }
    t1->utlz=t1->utlz/t1->npm;
    //printf("\n\nGene:\tEnergy consumption:%f\tUtlz:%f",t1->energy_consumption,t1->utlz);
}

//prints the best gene and its parameters
void printBestGene(popptr h1){
    geneptr best_gene=NULL;
    float best_utlz=0;
    printf("\n\nFinding gene with most utilization");
    for(int i=0;i<h1->n;i++){
        geneptr t1=h1->gene_list[i];
        printf("\n\tGene:%d utlz:%f",i,t1->utlz);
        if(t1->utlz>best_utlz){
            best_gene=t1;
            best_utlz=t1->utlz;
        }
    }
    printf("\nBest Gene's Utilization:%f and Energy Consumption:%f",best_gene->utlz,best_gene->energy_consumption);

}

void printBestGeneEnrg(popptr h1){
    geneptr best_gene=NULL;
    float best_enrg=INT_MAX;

    printf("\n\nFinding gene with least Energy Consumption");
    for(int i=0;i<h1->n;i++){
        geneptr t1=h1->gene_list[i];
        fitness1(t1);
        printf("\n\tGene:%d Energy Consumption:%f",i,t1->energy_consumption);
        if(t1->energy_consumption<best_enrg){
            best_gene=t1;
            best_enrg=t1->energy_consumption;
        }
    }
    printf("\nBest Gene's Utilization:%f and Energy Consumption:%f",best_gene->utlz,best_gene->energy_consumption);

}

geneptr searchBestGeneUtlz(popptr h1){
    geneptr best_gene=NULL;
    float best_utlz=0;

    for(int i=0;i<h1->n;i++){
        geneptr t1=h1->gene_list[i];
        fitness1(t1);
        if(t1->utlz>best_utlz){
            best_gene=t1;
            best_utlz=t1->utlz;
        }
    }

    return(best_gene);
}

geneptr searchBestGeneEnrg(popptr h1){
    geneptr best_gene=NULL;
    float best_enrg=INT_MAX;

    for(int i=0;i<h1->n;i++){
        geneptr t1=h1->gene_list[i];
        fitness1(t1);
        if(t1->energy_consumption<best_enrg){
            best_gene=t1;
            best_enrg=t1->energy_consumption;
        }
    }
    if(best_gene==NULL){
        printf("\nERROR: In SearchBestGeneEnrg: Gene not found");
        exit(0);
    }

    return(best_gene);
}

geneptr searchBestGeneEnrgWithMinNpm(popptr h1){
    int min_npm=INT_MAX;
    for(int i=0;i<h1->n;i++){
        geneptr t1=h1->gene_list[i];
        if(t1->npm<min_npm){
            min_npm=t1->npm;
        }
    }
    geneptr best_gene=NULL;
    float best_enrg=INT_MAX;

    for(int i=0;i<h1->n;i++){
        geneptr t1=h1->gene_list[i];
        if(t1->npm!=min_npm){
            continue;
        }
        fitness1(t1);
        if(t1->energy_consumption<best_enrg){
            best_gene=t1;
            best_enrg=t1->energy_consumption;
        }
    }

    return(best_gene);
}

geneptr searchBestGeneUtlzWithMinNpm(popptr h1){
    int min_npm=INT_MAX;
    for(int i=0;i<h1->n;i++){
        geneptr t1=h1->gene_list[i];
        if(t1->npm<min_npm){
            min_npm=t1->npm;
        }
    }
    geneptr best_gene=NULL;
    float best_utlz=0;

    for(int i=0;i<h1->n;i++){
        geneptr t1=h1->gene_list[i];
        if(t1->npm!=min_npm){
            continue;
        }
        fitness1(t1);
        if(t1->utlz>best_utlz){
            best_gene=t1;
            best_utlz=t1->utlz;
        }
    }

    return(best_gene);
}

void calculateContRes(geneptr t1){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            for(int k=0;k<vm->ncn;k++){
                cnptr cn=vm->cn_list[k];
                cn->norm_res=((cn->cpu)/(pm->cpu))*((cn->mem)/(pm->mem));
            }
        }
    }
}

void printStats(geneptr t1){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        printf("\n\nPM:%d : CPU :%f/%f Mem:%f/%f No of VM's:%d Utlz:%f",i,pm->cpu,pm->totcpu,pm->mem,pm->totmem,pm->nvm,pm->utlz);
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            printf("\n\tVM:%d : CPU:%f/%f Mem:%f/%f No of Containers:%d Utlz:%f",j,vm->cpu,vm->totcpu,vm->mem,vm->totmem,vm->ncn,vm->utlz);
            for(int k=0;k<vm->ncn;k++){
                cnptr cn=vm->cn_list[k];
                printf("\n\t\tContainer:%d : %d : CPU taken:%f Mem taken:%f",k,cn->ogi,cn->cpu,cn->mem);
            }
        }
    }
}
