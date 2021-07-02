#include "first_fit.h"

//first fit acc to the paper
void first_fit(geneptr t1){
    vmptr vm_list[CNT_SIZE];
    int no_of_vms=0;

    cnptr cn_list[CNT_SIZE];
    for(int i=0; i<CNT_SIZE; i++){
        cnptr new_cn=createCn(t1->test_containers[i]);
        cn_list[i]=new_cn;
    }

    no_of_vms=addCnListToVmListFF(cn_list,CNT_SIZE,vm_list,no_of_vms);

    pmptr pm_list[CNT_SIZE];
    int no_of_pms=0;

    no_of_pms=addVmListToPmListFF(vm_list,no_of_vms,pm_list,no_of_pms);

    addPmListToGene(pm_list,no_of_pms,t1);

}

//adds list of vms to list of pms, returns the size of list of pm
int addVmListToPmListFF(vmptr vm_list[],int vm_list_size,pmptr pm_list[],int pm_list_size){
    for(int i=0;i<vm_list_size;i++){
        vmptr new_vm = vm_list[i];
        pmptr selected_pm=NULL;

        for(int j=0;j<pm_list_size;j++){
            if(pmHasSpaceForVm(pm_list[j],new_vm)){
                selected_pm = pm_list[j];
                break;
            }
        }

        if(selected_pm==NULL){
            selected_pm=createPm();
            pm_list[pm_list_size]=selected_pm;
            pm_list_size++;
        }
        addVmToPm(new_vm,selected_pm);
    }
    return(pm_list_size);
}

//adds list of containers to vm list, returns size of list of vm
int addCnListToVmListFF(cnptr cn_list[],int cn_list_size,vmptr vm_list[],int vm_list_size){
    for(int i=0; i<cn_list_size; i++){
        cnptr new_cn=cn_list[i];
        vmptr selected_vm=NULL;

        for(int j=0;j<vm_list_size;j++){
            if(vmHasSpaceForContainer(vm_list[j],new_cn)){
                selected_vm = vm_list[j];
                break;
            }
        }
        if(selected_vm==NULL){
            int vmi=selectRandomVmForContainer(new_cn);
            selected_vm=createVm(vmi);
            vm_list[vm_list_size]=selected_vm;
            vm_list_size++;
        }
        addCnToVm(new_cn,selected_vm);
    }
    return(vm_list_size);
}


