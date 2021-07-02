#include "unpack_n_merge.h"

void unpackPm(geneptr t1){
    printf("\nIn UnpackPm:Start");
    fitness1(t1);
    float prob[t1->npm];
    int no_of_unpacks=(int)t1->npm*0.3;
    if(no_of_unpacks==0){
        no_of_unpacks=1;
    }
    for(int i=0;i<t1->npm;i++){
        prob[i]=calculateProbOfUnpack(t1->pm_list[i],t1);
        //printf("\nran:%f/%f\n",t1->pm_list[i]->utlz,prob[i]);
    }
    //sort prob desc
    for(int i=0;i<t1->npm;i++){
        int worst_pos=i;
        for(int j=i+1;j<t1->npm;j++){
            if(prob[j]>prob[worst_pos]){
                worst_pos=j;
            }
        }
        if(worst_pos==i){
            continue;
        }
        float temp=prob[i];
        prob[i]=prob[worst_pos];
        prob[worst_pos]=temp;
    }

    pmptr remove_pm_list[no_of_unpacks];
    int remove_pm_count=0;

    for(int i=0;i<no_of_unpacks;i++){
        float ran=(float)rand()/RAND_MAX;
        for(int j=0;j<t1->npm;j++){
            if(ran < prob[j] ){
                remove_pm_list[remove_pm_count]=t1->pm_list[j];
                prob[j]=-1;
                remove_pm_count++;
                break;
            }
        }
    }
    int removed_count=0;
    for(int i=0;i<remove_pm_count;i++){
        if(remove_pm_list[i]->utlz>UNPACK_THRSHLD){
            continue;
        }
        removePm(remove_pm_list[i],t1);
        removed_count++;
    }

    int cn_absent[CNT_SIZE];
    int absent_count=findAbsentContainers(t1,cn_absent);
    printf("\nIn UnpackPm: %d Containers Removed from %d pms",absent_count,removed_count);
    heuristics2(t1,cn_absent,absent_count);
    printf("\nIn UnpackPm:End");
}

float calculateProbOfUnpack(pmptr pm,geneptr t1){
    float prob_of_unpack=0;
    float sum_of_free_cpu=0;
    for(int i=0;i<t1->npm;i++){
        sum_of_free_cpu+=(1-t1->pm_list[i]->utlz);
    }
    prob_of_unpack=(1-pm->utlz)/sum_of_free_cpu;
    return(prob_of_unpack);
}

void mergeVm(geneptr t1){
    //printf("\nIn MergeVm:Start");
    //sort all vm asc
    sortVmbyRes(t1);
    //printStats(t1);
    //for each pm, either replace 2 smallest with a larger random vm or just replace 1 if possible
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        if(pm->nvm<1){
            removePm(pm,t1);
            i=i-1;//so that the pm replaced will be evaluated.
            continue;
        }
        if(pm->nvm>=2){
            vmptr first_vm=pm->vm_list[0];
            vmptr second_vm=pm->vm_list[1];

            if(can2VmsBeReplacedBy1(first_vm,second_vm,pm)==1){
                //printf("\nIn MergeVm:1");
                replace2VmsByRandom(first_vm,second_vm,pm);
                //printf("\nIn MergeVm:2");
                continue;
            }
        }

        vmptr vm=pm->vm_list[0];

        if(canThisVmBeReplacedByLargerVm(vm,pm)==1){
            //printf("\nIn MergeVm:3\t%f",pm->mem);
            replaceThisVmByLargerVm(vm,pm);
            //printf("\nIn MergeVm:4");
        }
    }
    printf("\nIn MergeVm:End");
}
