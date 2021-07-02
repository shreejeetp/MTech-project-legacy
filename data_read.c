#include "data_read.h"


int container_size=CNT_SIZE_INIT;

//reads the data into structure
void readCsv(){

    FILE *fpr;
    FILE *fpr2;
    FILE *fpr3;
    FILE *fpr4;
    FILE *fpr5;
    FILE *fpr6;
    FILE *fpr7;
    FILE *fpr8;
    FILE *fpr9;
    FILE *fpr10;
    FILE *fpr11;

    //char c;
    fpr=fopen("data/Container200_ten.csv","r");
    fpr2=fopen("data/Container200_twenty.csv","r");
    fpr3=fopen("data/VMConfig_ten.csv","r");
    fpr4=fopen("data/VMConfig_twenty.csv","r");
    fpr5=fopen("data/PMConfig_small.csv","r");
    fpr6=fopen("data/Container500_ten.csv","r");
    fpr7=fopen("data/Container500_twenty.csv","r");
    fpr8=fopen("data/Container1000_ten.csv","r");
    fpr9=fopen("data/Container1000_twenty.csv","r");
    fpr10=fopen("data/Container1500_ten.csv","r");
    fpr11=fopen("data/Container1500_twenty.csv","r");


    if(fpr==NULL||fpr2==NULL||fpr3==NULL||fpr4==NULL||fpr5==NULL||fpr6==NULL||fpr7==NULL||fpr8==NULL||fpr9==NULL||fpr10==NULL||fpr11==NULL){
        printf("\n ERROR in opening file\n");
    }
    //reading container file for synthetic vms
    int i=0;
    while(i<200){
        fscanf(fpr,"%f,%f\n",&data1.cont_syn200[i][0],&data1.cont_syn200[i][1]);
        i++;
    }
    //reading container file for real vms
    i=0;
    while(i<200){
        fscanf(fpr2,"%f,%f\n",&data1.cont_rea200[i][0],&data1.cont_rea200[i][1]);
        i++;
    }
    i=0;

    while(i<500){
        fscanf(fpr6,"%f,%f\n",&data1.cont_syn500[i][0],&data1.cont_syn500[i][1]);
        i++;
    }
    //reading container file for real vms
    i=0;
    while(i<500){
        fscanf(fpr7,"%f,%f\n",&data1.cont_rea500[i][0],&data1.cont_rea500[i][1]);
        i++;
    }

    i=0;

    while(i<1000){
        fscanf(fpr8,"%f,%f\n",&data1.cont_syn1000[i][0],&data1.cont_syn1000[i][1]);
        i++;
    }
    //reading container file for real vms
    i=0;
    while(i<1000){
        fscanf(fpr9,"%f,%f\n",&data1.cont_rea1000[i][0],&data1.cont_rea1000[i][1]);
        i++;
    }

    i=0;

    while(i<1500){
        fscanf(fpr10,"%f,%f\n",&data1.cont_syn1500[i][0],&data1.cont_syn1500[i][1]);
        i++;
    }
    //reading container file for real vms
    i=0;
    while(i<1500){
        fscanf(fpr11,"%f,%f\n",&data1.cont_rea1500[i][0],&data1.cont_rea1500[i][1]);
        i++;
    }


    //reading synthetic vm config
    i=0;
    while(i<10){
        fscanf(fpr3,"%f,%f\n",&data1.vm_syn[i][0],&data1.vm_syn[i][1]);
        i++;
    }

    //reading real vm config
    i=0;
    while(i<20){
        fscanf(fpr4,"%f,%f\n",&data1.vm_rea[i][0],&data1.vm_rea[i][1]);
        i++;
    }

    //reading pm config
    fscanf(fpr5,"%f\n%f\n%f",&data1.pm[0],&data1.pm[1],&data1.pm[2]);
    fclose(fpr);
    fclose(fpr2);
    fclose(fpr3);
    fclose(fpr4);
    fclose(fpr5);
    fclose(fpr6);
    fclose(fpr7);
    fclose(fpr8);
    fclose(fpr9);
    fclose(fpr10);
    fclose(fpr11);

}

//removes containers in structure that cant be fit in any vm, returns the no. of containers removed.
int removeUnfitDataContainers(){
    //initialize an empty bucket
    int removal_bucket[CNT_SIZE_INIT];

    for( int i = 0 ; i < CNT_SIZE_INIT ; i++ ){
        removal_bucket[i]=0;
    }
    int remove_count=0;
    //mark unfit containers for removal.
    for( int i = 0 ; i < CNT_SIZE_INIT ; i++ ){
        float cpu_req=data1.CNT_TYPE[i][0];
        float mem_req=data1.CNT_TYPE[i][1];

        int vm_found = 0;
        for( int j = 0 ; j < VM_SIZE ; j++ ){
            float vm_cpu = data1.VM_TYPE[j][0];
            float vm_mem = data1.VM_TYPE[j][1];

            if( ( cpu_req <= vm_cpu ) && ( mem_req <= vm_mem ) ){
                if( ( ( ( vm_cpu * 0.1 ) + cpu_req ) < data1.pm[0] ) && ( ( mem_req + 200 ) < data1.pm[1] ) ){
                    vm_found = 1;
                    break;
                }
            }
        }
        if( vm_found == 0 ){
            removal_bucket[i]=1;
            remove_count++;
        }
    }

    //remove marked containers.
    float new_cont_arr[CNT_SIZE_INIT][2];
    int j=0;
    for( int i = 0 ; i < CNT_SIZE_INIT ; i++ ){
        if( removal_bucket[i] == 0 ){
            new_cont_arr[j][0]=data1.CNT_TYPE[i][0];
            new_cont_arr[j][1]=data1.CNT_TYPE[i][1];
            j++;
        }
    }

    for( int i = 0 ; i < j ; i++ ){
        data1.CNT_TYPE[i][0]=new_cont_arr[i][0];
        data1.CNT_TYPE[i][1]=new_cont_arr[i][1];
    }
    for( int i = j ; i < CNT_SIZE_INIT ; i++ ){
        data1.CNT_TYPE[i][0]=0;
        data1.CNT_TYPE[i][1]=0;
    }
    container_size=j;

    return(remove_count);
}

