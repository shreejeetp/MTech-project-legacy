#include "crossover.h"

void crossMut(popptr p1){
    int prob_crs=rand()%10;

    if(prob_crs<PROB_CROSSOVER){
        crossover(p1);
    }
    else{
        mutation(p1);
    }
}

void mutation(popptr p1){
    printf("\n Mutation Started");
    geneptr parent1=naryTrnment(p1);

    geneptr child=createCopyGene(parent1);

    int max_vms_to_be_removed=20;
    int min_vms_to_be_removed=5;

    int vms_to_be_removed=min_vms_to_be_removed+(rand()%(max_vms_to_be_removed-min_vms_to_be_removed));
    printf("\n In Mutation: Removing %d Vms",vms_to_be_removed);

    int removed_count=0;
    for(int i=0;(i<child->npm)&&(removed_count<vms_to_be_removed);i++){
        pmptr pm=child->pm_list[i];
        for(int j=0;(j<pm->nvm)&&(removed_count<vms_to_be_removed);j++){
            vmptr vm=pm->vm_list[j];
            if(vm->utlz>VM_UNPACK_THRSHLD){
                continue;
            }
            removeVm(vm,pm);
            removed_count++;
        }
    }

    int cn_absent[CNT_SIZE];
    int absent_count=findAbsentContainers(child,cn_absent);
    printf("\nIn Mutation: %d Containers Removed from %d vms",absent_count,removed_count);
    heuristics2(child,cn_absent,absent_count);
    printf("\nIn Mutation:End");
    postCrossMut(child,p1);
}

void crossover(popptr p1){
    printf("\nCrossover Started");
    geneptr parent1=naryTrnment(p1);
    geneptr parent2=naryTrnment(p1);

    while(parent1==parent2){
        parent2=naryTrnment(p1);
    }

    printf("\n\nparent 1 utilization:%f,parent 2 utilization:%f",parent1->utlz,parent2->utlz);

    geneptr child1=crossover2(parent1, parent2, 0);
    geneptr child2=crossover2(parent1, parent2, 1);

    postCrossMut(child1,p1);
    postCrossMut(child2,p1);

    printf("\nCrossover Done");
}


geneptr naryTrnment(popptr h1){
    geneptr genelist[TRN_SIZE];

    for(int i=0;i<TRN_SIZE;i++){
        int ran=rand()%h1->n;
        genelist[i]=h1->gene_list[ran];
    }

    float best_utlz=0;
    geneptr best_gene=NULL;
    int random_gene_no=rand()%TRN_SIZE;
    geneptr random_gene=genelist[random_gene_no];

    for(int i=0;i<TRN_SIZE;i++){
        if( best_utlz < genelist[i]->utlz ){
            best_gene=genelist[i];
            best_utlz=genelist[i]->utlz;
        }
    }

    int dc=rand()%10;
    if(dc<6){
        return best_gene;
    }
    return random_gene;
}

//sub function of crossover, which takes 2 parents as input and returns a child pointer. if sort_pm_by_cpu is 1 pms are sorted by cpu.
geneptr crossover2(geneptr parent1,geneptr parent2,int sort_pm_by_cpu){
    printf("\ncrossover2 started");
    geneptr child=create_gene();

    parent1=createCopyGene(parent1);
    parent2=createCopyGene(parent2);

    sortPmByUtlz(parent1,sort_pm_by_cpu);
    sortPmByUtlz(parent2,sort_pm_by_cpu);
    fitness(parent1);
    fitness(parent2);

    int min_npm = parent1->npm <= parent2->npm ? parent1->npm : parent2->npm;
    pmptr pm_list_tobe_allocated[min_npm];

    //creates a list of pms to be allocated to the child
    for(int i=0;i<min_npm;i++){
        if(sort_pm_by_cpu==1){
            if( parent1->pm_list[i]->utlz >= parent2->pm_list[i]->utlz ){
                pm_list_tobe_allocated[i]=createCopyPm(parent1->pm_list[i]);
            }
            else{
                pm_list_tobe_allocated[i]=createCopyPm(parent2->pm_list[i]);
            }
        }
        else{
            if( parent1->pm_list[i]->mem_utlz >= parent2->pm_list[i]->mem_utlz ){
                pm_list_tobe_allocated[i]=createCopyPm(parent1->pm_list[i]);
            }
            else{
                pm_list_tobe_allocated[i]=createCopyPm(parent2->pm_list[i]);
            }
        }
    }
    addPmListToGene(pm_list_tobe_allocated,min_npm,child);
    int duplicates_removed=removeDuplicateContainersFromGene(child);
    printf("\nDuplicates removed:%d",duplicates_removed);
    //printStats(child);
    fitness1(child);
    countContainers(child);

    int cn_absent[CNT_SIZE];
    int absent_count=findAbsentContainers(child,cn_absent);
    //printStats(child);

    printf("\nabsent count:%d",absent_count);

    if(absent_count!=0){
        //printf("\nIn Crossover: Before heuristics");
        heuristics2(child,cn_absent,absent_count);
        //printf("\nIn Crossover: After heuristics");
    }

    countContainers(child);
    integrity_all_containers(child);

    free(parent1);
    free(parent2);
    return child;
}

void postCrossMut(geneptr child,popptr h1){
    unpackPm(child);
    //fitness(child);
    mergeVm(child);
    //unpack_vm(child);
    //printstats(child);
    removeEmptyVmAndPms(child);
    integrity_all_containers(child);
    fitness(child);
    //remove_empty_pm_vm(child);
    if(uniqueness(child,h1)==1){
            removeGene(h1,findWorstGene(h1));
            addGene(h1,child);
    }
}
