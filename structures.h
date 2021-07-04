#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED
#include "macros.h"

struct data{
    float cont_syn200[200][2];
    float cont_rea200[200][2];
    float cont_syn500[500][2];
    float cont_rea500[500][2];
    float cont_syn1000[1000][2];
    float cont_rea1000[1000][2];
    float cont_syn1500[1500][2];
    float cont_rea1500[1500][2];
    float vm_syn[10][2];
    float vm_rea[20][2];
    float pm[3];
    int test_containers[CNT_SIZE_INIT];
    int containers_sample[200][2];
}data1;

struct cn{
    int ogi;//og index
    float cpu;
    float mem;
    float norm_res;

};

struct vm{
    int ogi;
    int ncn;
    float cpu;
    float mem;
    float utlz;
    float mem_utlz;
    struct cn* cn_list[CNT_SIZE_INIT];
    float totcpu;
    float totmem;
    float norm_res;
};

struct pm{
    int nvm;
    float cpu;
    float mem;
    float enrg;
    float totcpu;
    float totmem;
    struct vm* vm_list[CNT_SIZE_INIT];
    float utlz;
    float energy_consumption;
    float mem_utlz;

};

struct gene{
    float utlz;
    float energy_consumption;
    int npm;
    struct pm* pm_list[CNT_SIZE_INIT];
    int test_containers[CNT_SIZE_INIT];
};

struct population{
    struct gene* gene_list[NO_POP];
    int n;
}*head_population;


typedef struct population* popptr;
typedef struct gene* geneptr;
typedef struct pm* pmptr;
typedef struct vm* vmptr;
typedef struct cn* cnptr;



#endif // STRUCTURES_H_INCLUDED
