#include "population.h"

//creates a new population of genes and stores them in head_population
void createPop(){
    //initialize headpointer
    head_population=(popptr)malloc(sizeof(struct population));
    head_population->n=0;
    popptr h1=head_population;

    //create genes
    for(int i=0;i<NO_POP;i++){
        printf("\nCreating Gene:%d",i);
        geneptr t1=create_gene();
        generate_random_containers(t1);
        first_fit(t1);
        fitness(t1);
        integrity_all_containers(t1);
        addGene(h1,t1);
        printf("\n\nFinished creating Gene:%d\n",i);
    }

    printBestGene(h1);
}

//generates random combinations of containers and stores them in gene->test_containers.
void generate_random_containers(geneptr t1){
    int i=0;
    int n=CNT_SIZE;
    int check_bucket[n];
    while(i<n){
        check_bucket[i]=0;
        i++;
    }
    i=0;
    int max_no_of_random_tries=CNT_SIZE/2;
    while(i<n){
        int ran=(rand()%n);
        int tries=0;
        while(check_bucket[ran]!=0){
            ran=(rand()%n);
            if( tries > max_no_of_random_tries ){
                for(int j = 0 ; j < n ; j++){
                    if( check_bucket[j] == 0 ){
                        ran=j;
                        break;
                    }
                }
            }
            tries++;
        }
        t1->test_containers[i]=ran;
        check_bucket[ran]=1;
        //printf("\n%d added in test_containers\n",ran);
        i++;
    }

}

void elitism(popptr h1){
    geneptr gene_list[ELITISM_COUNT];

    for(int i=0;i<ELITISM_COUNT;i++){
        geneptr best_gene=searchBestGeneEnrgWithMinNpm(h1);
        gene_list[i]=createCopyGene(best_gene);
        fitness1(gene_list[i]);
        removeGene(h1,best_gene);
    }
    emptyPopulation(h1);

    for(int i=0;i<ELITISM_COUNT;i++){
        addGene(h1,gene_list[i]);
    }
}

void emptyPopulation(popptr h1){
    for(int i=0;i<h1->n;i++){
        free(h1->gene_list[i]);
        h1->gene_list[i]=NULL;
    }
    h1->n=0;
}


