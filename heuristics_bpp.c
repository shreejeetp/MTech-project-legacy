#include "heuristics_bpp.h"

void heuristics2(geneptr t1,int cn_absent[],int absent_count){
    //printf("\nheuristics started");
    sortContainersByRes(t1);
    //complete fit
    for(int i=0;i<absent_count;i++){
        vmptr complete_fit_vm=NULL;
        cnptr temp_cn=createCn(cn_absent[i]);
        complete_fit_vm=findVmForCnCompleteFit(t1,temp_cn);
        if(complete_fit_vm!=NULL){
            //printf("\n1");
            cnptr new_cn=createCn(cn_absent[i]);
            addCnToVmAndPm(new_cn,complete_fit_vm,findPmByVm(complete_fit_vm,t1));
            if(i!=absent_count-1){
                cn_absent[i]=cn_absent[absent_count-1];
                i--;
            }
            absent_count--;
        }
        free(temp_cn);
    }

    //better fit
    for(int i=0;i<absent_count;i++){
        //dbl replacement
        vmptr vm_rep=NULL;
        int dbl_rep_cn_list[2];
        cnptr temp_cn=createCn(cn_absent[i]);
        vm_rep=dblReplacementIsPossible(temp_cn,dbl_rep_cn_list,t1);
        if(vm_rep!=NULL){
            //printf("\n2");
            removeContainer(dbl_rep_cn_list[0],t1);
            removeContainer(dbl_rep_cn_list[1],t1);

            cnptr new_cn=createCn(cn_absent[i]);
            //printf("\ncontainer added:cpu:%f/%f\tmem:%f/%f",new_cn->cpu,vm_rep->cpu,new_cn->mem,vm_rep->mem);
            addCnToVmAndPm(new_cn,vm_rep,findPmByVm(vm_rep,t1));

            heuristics2(t1,dbl_rep_cn_list,2);
            if(i!=absent_count-1){
                //printf("\nbefore shift:%d",cn_absent[i]);
                cn_absent[i]=cn_absent[absent_count-1];
                //printf("\nafter shift:%d",cn_absent[i]);
                i--;
            }
            absent_count--;
            free(temp_cn);
            continue;
        }
        //single replacement
        vm_rep=NULL;
        int single_rep_cn_list[1];
        vm_rep=singleReplacementIsPossible(temp_cn,single_rep_cn_list,t1);
        if(vm_rep!=NULL){
            //printf("\n3");
            removeContainer(single_rep_cn_list[0],t1);

            cnptr new_cn=createCn(cn_absent[i]);
            pmptr pm_rep=findPmByVm(vm_rep,t1);
            //printf("\ncontainer added:cpu:%f/%f\tmem:%f/%f",new_cn->cpu,vm_rep->cpu,new_cn->mem,vm_rep->mem);
            addCnToVmAndPm(new_cn,vm_rep,pm_rep);

            heuristics2(t1,single_rep_cn_list,1);
            if(i!=absent_count-1){
                //printf("\nbefore shift:%d",cn_absent[i]);
                cn_absent[i]=cn_absent[absent_count-1];
                //printf("\nafter shift:%d",cn_absent[i]);
                i--;
            }
            absent_count--;
        }
        free(temp_cn);

    }
    //printf("\n4");
    allocate_list_ff_rc(t1,cn_absent,absent_count);
    //printf("\n5");
}

//find a better fit single replacement for the container passed
vmptr singleReplacementIsPossible(cnptr cn_to_be_replaced,int single_rep_cn_list[],geneptr t1){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];

        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            for(int k=0;k<vm->ncn;k++){
                cnptr cn1=vm->cn_list[k];

                float vm_cpu_after_rep=vm->cpu + cn1->cpu - cn_to_be_replaced->cpu;
                float vm_mem_after_rep=vm->mem + cn1->mem - cn_to_be_replaced->mem;

                float pm_cpu_after_rep=pm->cpu + cn1->cpu - cn_to_be_replaced->cpu;
                float pm_mem_after_rep=pm->mem + cn1->mem - cn_to_be_replaced->mem;

                if((vm_cpu_after_rep>=0)&&(vm_mem_after_rep>=0)&&(pm_cpu_after_rep>=0)&&(pm_mem_after_rep>=0)&&(vm_cpu_after_rep<vm->cpu)){
                    single_rep_cn_list[0]=cn1->ogi;
                    return(vm);
                }

            }
        }
    }
    return(NULL);
}

//finds a better fit double replacement for the container passed.
vmptr dblReplacementIsPossible(cnptr cn_to_be_replaced,int dbl_rep_cn_list[],geneptr t1){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];

        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            for(int k=0;k<vm->ncn;k++){
                cnptr cn1=vm->cn_list[k];
                for(int k2=k+1;k2<vm->ncn;k2++){
                    cnptr cn2=vm->cn_list[k2];
                    float combined_cpu=cn1->cpu+cn2->cpu;
                    float combined_mem=cn1->mem+cn2->mem;

                    float vm_cpu_after_rep=vm->cpu+combined_cpu-cn_to_be_replaced->cpu;
                    float vm_mem_after_rep=vm->mem+combined_mem-cn_to_be_replaced->mem;

                    float pm_cpu_after_rep=pm->cpu+combined_cpu-cn_to_be_replaced->cpu;
                    float pm_mem_after_rep=pm->mem+combined_mem-cn_to_be_replaced->mem;

                    if((vm_cpu_after_rep>=0)&&(vm_mem_after_rep>=0)&&(pm_cpu_after_rep>=0)&&(pm_mem_after_rep>=0)&&(vm_cpu_after_rep<vm->cpu)){
                        dbl_rep_cn_list[0]=cn1->ogi;
                        dbl_rep_cn_list[1]=cn2->ogi;

                        return(vm);
                    }


                }
            }
        }
    }
    return(NULL);
}

//return the vm which can be completely filled by the container, returns NULL if no vm can be completely filled.
vmptr findVmForCnCompleteFit(geneptr t1,cnptr cn){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        if(pmHasSpaceForContainer(pm,cn)!=1){
            continue;
        }
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];

            float vm_cpu_after_add=vm->cpu-cn->cpu;
            float vm_mem_after_add=vm->mem-cn->mem;

            if((vm_cpu_after_add>=0)&&(vm_mem_after_add>=0)&&(vm->cpu-cn->cpu<=0.01*vm->cpu)){
                return(vm);
            }
        }
    }
    return(NULL);
}

//first fit/random create first fit heuristics to add a container to the gene
void ff_rc_heuristics(cnptr cn,geneptr t1){
    vmptr selected_vm=findVmForCn(t1,cn);

    //if theres any vm that can contain both
    if(selected_vm!=NULL){
        addCnToVmAndPm(cn,selected_vm,findPmByVm(selected_vm,t1));
        return;
    }

    //create new vm
    int vm_ogi=selectRandomVmForContainer(cn);
    vmptr new_vm=createVm(vm_ogi);
    addCnToVm(cn,new_vm);

    //find pm which has space for this vm
    pmptr selected_pm=findPmForVm(t1,new_vm);

    //if there is an existing pm that can have it
    if(selected_pm!=NULL){
        addVmToPm(new_vm,selected_pm);
        return;
    }
    //if there's not
    pmptr new_pm=createPm(t1);
    addVmToPm(new_vm,new_pm);
    addPmToGene(new_pm,t1);
    fitness1(t1);
}

//allocates list of container's index to gene by ff&rc/ff
void allocate_list_ff_rc(geneptr t1,int cn_list[],int list_length){
    for(int i=0;i<list_length;i++){
        cnptr newcn=createCn(cn_list[i]);
        ff_rc_heuristics(newcn,t1);
    }
}

//return the vm which
