#include "crud_vm.h"

//creates a container pointer
cnptr createCn(int cni){
    cnptr cn=(cnptr)malloc(sizeof(struct cn));
    cn->cpu=data1.CNT_TYPE[cni][0];
    cn->mem=data1.CNT_TYPE[cni][1];
    cn->norm_res=0;
    cn->ogi=cni;
    return(cn);

}

//if vm has space for container, returns 1, else 0
int vmHasSpaceForContainer(vmptr vm,cnptr cn){
    if((vm->cpu>=cn->cpu)&&(vm->mem>=cn->mem)){
        return(1);
    }
    return(0);
}

//if pm has space for vm, returns 1, otherwise 0
int pmHasSpaceForVm(pmptr pm,vmptr vm){
    if( (pm->cpu >= ((vm->totcpu - vm->cpu)+(vm->totcpu*0.1))) &&(pm->mem >= (vm->totmem-vm->mem+200)) ){
        return(1);
    }
    return(0);
}

//find if pm has space for a container
int pmHasSpaceForContainer(pmptr pm,cnptr cn){
    if((pm->cpu>=cn->cpu)&&(pm->mem>=cn->mem)){
        return(1);
    }
    return(0);
}

//selects random vm for a container
int selectRandomVmForContainer(cnptr cn){
    float cpu_req=cn->cpu;
    float mem_req=cn->mem;

    int vm_ogi=rand()%VM_SIZE;
    float vm_cpu=data1.VM_TYPE[vm_ogi][0];
    float vm_mem=data1.VM_TYPE[vm_ogi][1];
    int max_no_of_random_tries=VM_SIZE*2;
    int tries=0;
    while((data1.pm[0]<(vm_cpu*1.1)) || (data1.pm[1]<(vm_mem+200)) || (vm_mem<mem_req) || (vm_cpu<cpu_req)){
        vm_ogi=rand()%VM_SIZE;
        vm_cpu=data1.VM_TYPE[vm_ogi][0];
        vm_mem=data1.VM_TYPE[vm_ogi][1];
        if( tries > max_no_of_random_tries ){
            for( int i=0 ; i < VM_SIZE ; i++ ){
                vm_ogi=i;
                vm_cpu=data1.VM_TYPE[vm_ogi][0];
                vm_mem=data1.VM_TYPE[vm_ogi][1];

                if( ( data1.pm[0] >= (vm_cpu*1.1) ) || ( data1.pm[1] >= (vm_mem+200) ) || ( vm_mem >= mem_req) || ( vm_cpu >= cpu_req ) ){
                    break;
                }
            }
        }

        tries++;
    }
    return(vm_ogi);
}

//creates a vm pointer
vmptr createVm(int vmi){
    vmptr vm=(vmptr)malloc(sizeof(struct vm));
    vm->cpu=data1.VM_TYPE[vmi][0];
    vm->mem=data1.VM_TYPE[vmi][1];
    vm->mem_utlz=0;
    vm->ncn=0;
    vm->norm_res=0;
    vm->ogi=vmi;
    vm->totcpu=data1.VM_TYPE[vmi][0];
    vm->totmem=data1.VM_TYPE[vmi][1];
    vm->utlz=0;

    //initialize empty cn list
    for(int i=0;i<CNT_SIZE_INIT;i++){
        vm->cn_list[i]=NULL;
    }
    return(vm);

}

//creates a pm pointer
pmptr createPm(){
    pmptr pm=(pmptr)malloc(sizeof(struct pm));
    pm->cpu=data1.pm[0];
    pm->mem=data1.pm[1];
    pm->enrg=data1.pm[2];
    pm->nvm=0;
    pm->energy_consumption=0;
    pm->totcpu=data1.pm[0];
    pm->totmem=data1.pm[1];
    pm->mem_utlz=0;
    pm->utlz=0;

    for(int i=0;i<CNT_SIZE_INIT;i++){
        pm->vm_list[i]=NULL;
    }
    return(pm);
}

//adds a cn to vm
void addCnToVm(cnptr cn,vmptr vm){
    vm->cpu-=(cn->cpu);
    vm->mem-=(cn->mem);

    vmSizeIntegrityCheck(vm);

    vm->cn_list[vm->ncn]=cn;
    vm->ncn++;
}

//adds vm to pm
void addVmToPm(vmptr vm,pmptr pm){
    pm->cpu-=((vm->totcpu-vm->cpu)+(vm->totcpu*0.1));
    pm->mem-=(vm->totmem-vm->mem+200);

    pmSizeIntegrityCheck(pm);

    pm->vm_list[pm->nvm]=vm;
    pm->nvm++;
}

//sorts all pm in gene by their utilization.
void sortPmByUtlz(geneptr t1,int sort_by_cpu){
    fitness1(t1);

    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        pmptr best_pm=pm;
        int best_pm_index=i;
        for(int j=i+1;j<t1->npm;j++){
            pmptr pm2=t1->pm_list[j];

            if(sort_by_cpu==1){
                if(pm2->utlz>best_pm->utlz){
                    best_pm=pm2;
                    best_pm_index=j;
                }
            }
            else{
                if(pm2->mem_utlz>best_pm->mem_utlz){
                    best_pm=pm2;
                    best_pm_index=j;
                }
            }
        }
        if(best_pm_index==i){
            continue;
        }
        swapPm(t1,i,best_pm_index);
    }
}

void swapPm(geneptr t1,int x,int y){
    if((x>=t1->npm)||(y>=t1->npm)||(x<0)||(y<0)||(t1->pm_list[x]==NULL)||(t1->pm_list[y]==NULL)){
        printf("\nERROR: SWAP PM: Cant find the pm passed!");
        exit(0);
    }

    pmptr temp_pm=t1->pm_list[x];
    t1->pm_list[x]=t1->pm_list[y];
    t1->pm_list[y]=temp_pm;
}

void removeContainer(int cni,geneptr t1){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        //printf("\npm:%d",pm->nvm);
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            //printf("\n%d vm:%d",j,vm->ncn);
            for(int k=0;k<vm->ncn;k++){
                cnptr cn=vm->cn_list[k];
                //printf("\nbefore found:%d/%d",cn->ogi,cni);
                if(cn->ogi==cni){
                    //printf("\nin found");
                    pm->cpu+=cn->cpu;
                    pm->mem+=cn->mem;
                    vm->cpu+=cn->cpu;
                    vm->mem+=cn->mem;
                    if(k!=vm->ncn-1){
                        swapContainers(vm,k,vm->ncn-1);
                    }
                    //printf("\nbefore free");

                    free(cn);

                    vm->cn_list[vm->ncn-1]=NULL;
                    vm->ncn--;
                    //printf("\nafter free");
                    return;
                }
            }
        }
    }
    printf("ERROR: In RemoveContainer: Cant find Container");
    exit(0);
}

void swapContainers(vmptr vm,int x,int y){
    if((x>=vm->ncn)||(y>=vm->ncn)||(x<0)||(y<0)||(vm->cn_list[x]==NULL)||(vm->cn_list[y]==NULL)){
        printf("\nERROR: SWAP Container: Cant find the container passed!");
        exit(0);
    }
    cnptr temp_cn=vm->cn_list[x];
    vm->cn_list[x]=vm->cn_list[y];
    vm->cn_list[y]=temp_cn;
}

//sorts all the containers in gene acc to their normalised resources.
void sortContainersByRes(geneptr t1){
    calculateContRes(t1);

    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            for(int k=0;k<vm->ncn;k++){
                cnptr cn=vm->cn_list[k];
                cnptr best_cn=cn;
                int best_pos=k;
                for(int k1=k+1;k1<vm->ncn;k1++){
                    cnptr cn2=vm->cn_list[k1];
                    if(cn2->norm_res < best_cn->norm_res){
                        best_cn=cn2;
                        best_pos=k1;
                    }
                }
                if(k==best_pos){
                    continue;
                }
                swapContainers(vm,k,best_pos);
            }
        }
    }
}

vmptr findVmForCn(geneptr t1,cnptr cn){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        if(pmHasSpaceForContainer(pm,cn)!=1){
            continue;
        }
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            if(vmHasSpaceForContainer(vm,cn)){
                return(vm);
            }
        }
    }
    return(NULL);
}

pmptr findPmForVm(geneptr t1,vmptr vm){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        if(pmHasSpaceForVm(pm,vm)){
            return(pm);
        }
    }
    return(NULL);
}

pmptr findPmByVm(vmptr vm,geneptr t1){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;j<pm->nvm;j++){
            vmptr vm1=pm->vm_list[j];
            if(vm==vm1){
                return(pm);
            }
        }
    }
    printf("ERROR:In findVmByPm: Cant find Pm");
    exit(0);
}

void addCnToVmAndPm(cnptr cn, vmptr vm, pmptr pm){
    vm->cpu-=(cn->cpu);
    vm->mem-=(cn->mem);
    pm->cpu-=(cn->cpu);
    pm->mem-=(cn->mem);

    vmSizeIntegrityCheck(vm);
    pmSizeIntegrityCheck(pm);

    vm->cn_list[vm->ncn]=cn;
    vm->ncn++;
}

pmptr createCopyPm(pmptr pm){
    pmptr cpy_pm=createPm();
    for(int i=0;i<pm->nvm;i++){
        vmptr vm=pm->vm_list[i];
        vmptr cpy_vm=createVm(vm->ogi);

        for(int j=0;j<vm->ncn;j++){
            cnptr cn=vm->cn_list[j];
            cnptr cpy_cn=createCn(cn->ogi);
            addCnToVm(cpy_cn,cpy_vm);
        }
        addVmToPm(cpy_vm,cpy_pm);
    }
    return(cpy_pm);
}

vmptr createCopyVm(vmptr vm){
    vmptr cpy_vm=createVm(vm->ogi);
    for(int i=0;i<vm->ncn;i++){
        cnptr cn=vm->cn_list[i];
        cnptr cpy_cn=createCn(cn->ogi);

        addCnToVm(cpy_cn,cpy_vm);
    }
    return(cpy_vm);
}

void removeVm(vmptr vm1, pmptr pm){
    for(int i=0;i<pm->nvm;i++){
        vmptr vm=pm->vm_list[i];
        if(vm==vm1){
            if(i!=pm->nvm-1){
                swapVm(pm,i,pm->nvm-1);
            }
            pm->cpu+=(vm->totcpu-vm->cpu)+(vm->totcpu*0.1);
            pm->mem+=(vm->totmem-vm->mem)+(200);
            free(vm);
            pm->vm_list[pm->nvm-1]=NULL;
            pm->nvm--;
            return;
        }
    }
    printf("\nERROR: In RemoveVm: Cant find vm");
    exit(0);
}

void removePm(pmptr pm1,geneptr t1){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        if(pm==pm1){
            if(i!=t1->npm-1){
                swapPm(t1,i,t1->npm-1);
            }
            free(pm);
            t1->pm_list[t1->npm-1]=NULL;
            t1->npm--;
            return;
        }
    }
    printf("\nERROR: In RemovePm: Cant find pm");
    exit(0);
}

void sortVmbyRes(geneptr t1){
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            vmptr best_vm=vm;
            int best_pos=j;
            for(int k=j+1;k<pm->nvm;k++){
                vmptr vm2=pm->vm_list[k];
                if((vm2->totcpu < best_vm->totcpu)){
                    best_vm=vm2;
                    best_pos=k;
                }
            }
            if(best_pos==j){
                continue;
            }
            swapVm(pm,j,best_pos);
        }
    }
}

void swapVm(pmptr pm, int x, int y){
    if((x<0)||(y<0)||(x>=pm->nvm)||(y>=pm->nvm)||(pm->vm_list[x]==NULL)||(pm->vm_list[y]==NULL)){
        printf("\nERROR: In swap vm: Cant find the vm");
        exit(0);
    }
    vmptr temp_vm=pm->vm_list[x];
    pm->vm_list[x]=pm->vm_list[y];
    pm->vm_list[y]=temp_vm;
}

int can2VmsBeReplacedBy1(vmptr first_vm, vmptr second_vm, pmptr pm){
    float combined_cpuocc=first_vm->totcpu+second_vm->totcpu-first_vm->cpu-second_vm->cpu;
    float combined_memocc=first_vm->totmem+second_vm->totmem-first_vm->mem-second_vm->mem;
    float combined_cpu=first_vm->totcpu+second_vm->totcpu;

    for(int i=0;i<VM_SIZE;i++){
        float vm_cpu=data1.VM_TYPE[i][0];
        float vm_mem=data1.VM_TYPE[i][1];

        if((vm_cpu>=combined_cpuocc)&&(vm_mem>=combined_memocc)&&((pm->cpu+(combined_cpu*0.1)-(vm_cpu*0.1)) >= 0 ) ){
            return(1);
        }
    }
    return(0);
}

int canThisVmBeReplacedByLargerVm(vmptr vm,pmptr pm){
    float memocc=vm->totmem-vm->mem;

    for(int i=0;i<VM_SIZE;i++){
        float vm_cpu=data1.VM_TYPE[i][0];
        float vm_mem=data1.VM_TYPE[i][1];

        if((vm->ogi!=i)&&(vm_cpu>=vm->totcpu)&&(vm_mem>=memocc)&&((pm->cpu+(vm->totcpu*0.1)-(vm_cpu*0.1)) > 0 ) ){
            return(1);
        }
    }
    return(0);
}

void replace2VmsByRandom(vmptr first_vm,vmptr second_vm,pmptr pm){
    float combined_cpuocc=first_vm->totcpu+second_vm->totcpu-first_vm->cpu-second_vm->cpu;
    float combined_memocc=first_vm->totmem+second_vm->totmem-first_vm->mem-second_vm->mem;
    float combined_cpu=first_vm->totcpu+second_vm->totcpu;

    int vm_ogi=rand()%VM_SIZE;
    float vm_cpu=data1.VM_TYPE[vm_ogi][0];
    float vm_mem=data1.VM_TYPE[vm_ogi][1];
    int max_no_of_random_tries=VM_SIZE*2;
    int tries=0;
    while((vm_cpu<combined_cpuocc)||(vm_mem<combined_memocc)||((pm->cpu+(combined_cpu*0.1)-(vm_cpu*0.1)) < 0 )){
        vm_ogi=rand()%VM_SIZE;
        vm_cpu=data1.VM_TYPE[vm_ogi][0];
        vm_mem=data1.VM_TYPE[vm_ogi][1];
        if( tries > max_no_of_random_tries ){
            for( int i=0 ; i < VM_SIZE ; i++ ){
                vm_ogi=i;
                vm_cpu=data1.VM_TYPE[vm_ogi][0];
                vm_mem=data1.VM_TYPE[vm_ogi][1];

                if( (vm_cpu>=combined_cpuocc)&&(vm_mem>=combined_memocc)&&((pm->cpu+(combined_cpu*0.1)-(vm_cpu*0.1)) >= 0 ) ){
                    break;
                }
            }
        }

        tries++;
    }

    vmptr dup_first_vm=createCopyVm(first_vm);
    vmptr dup_second_vm=createCopyVm(second_vm);


    removeVm(first_vm,pm);
    removeVm(second_vm,pm);

    vmptr new_vm=createVm(vm_ogi);
    for(int i=0;i<dup_first_vm->ncn;i++){
        cnptr cn=dup_first_vm->cn_list[i];
        cnptr temp_cn=createCn(cn->ogi);
        addCnToVm(temp_cn,new_vm);
    }

    for(int i=0;i<dup_second_vm->ncn;i++){
        cnptr cn=dup_second_vm->cn_list[i];
        cnptr temp_cn=createCn(cn->ogi);
        addCnToVm(temp_cn,new_vm);
    }
    addVmToPm(new_vm,pm);
    free(dup_first_vm);
    free(dup_second_vm);
}

void replaceThisVmByLargerVm(vmptr vm,pmptr pm){
    //printf("\nIn Replace1ByLarger1:Start");
    float memocc=vm->totmem-vm->mem;

    int vm_ogi=rand()%VM_SIZE;
    float vm_cpu=data1.VM_TYPE[vm_ogi][0];
    float vm_mem=data1.VM_TYPE[vm_ogi][1];
    int max_no_of_random_tries=VM_SIZE*2;
    int tries=0;
    while((vm->ogi==vm_ogi)||(vm_cpu < vm->totcpu)||(vm_mem < memocc)||((pm->cpu+(vm->totcpu*0.1)-(vm_cpu*0.1)) < 0 )){
        vm_ogi=rand()%VM_SIZE;
        vm_cpu=data1.VM_TYPE[vm_ogi][0];
        vm_mem=data1.VM_TYPE[vm_ogi][1];
        if( tries > max_no_of_random_tries ){
            for( int i=0 ; i < VM_SIZE ; i++ ){
                vm_ogi=i;
                vm_cpu=data1.VM_TYPE[vm_ogi][0];
                vm_mem=data1.VM_TYPE[vm_ogi][1];

                if( (vm->ogi!=vm_ogi)&&(vm_cpu>=vm->totcpu)&&(vm_mem>=memocc)&&((pm->cpu+(vm->totcpu*0.1)-(vm_cpu*0.1)) >= 0 ) ){
                    break;
                }
            }
        }

        tries++;
    }
   // printf("\nIn Replace1ByLarger1:vm:%d/%d,cpu:%f/%f,mem:%f/%f",vm->ogi,vm_ogi,vm->totcpu,data1.VM_TYPE[vm->ogi][0],vm->totmem,data1.VM_TYPE[vm->ogi][1]);
    vmptr dup_vm=createCopyVm(vm);
    removeVm(vm,pm);
    vmptr new_vm=createVm(vm_ogi);

    for(int i=0;i<dup_vm->ncn;i++){
        cnptr cn=dup_vm->cn_list[i];
        cnptr temp_cn=createCn(cn->ogi);
        addCnToVm(temp_cn,new_vm);
    }


    addVmToPm(new_vm,pm);
    free(dup_vm);
}

void removeEmptyVmAndPms(geneptr t1){
    //remove empty vms
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;j<pm->nvm;j++){
            vmptr vm=pm->vm_list[j];
            if(vm->ncn==0){
                removeVm(vm,pm);
                j--;
            }
        }
    }

    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        if(pm->nvm==0){
            removePm(pm,t1);
            i--;
        }
    }
}
