#include "macros.h"
#include "structures.h"
#include "data_read.h"
#include "population.h"
#include "evaluation.h"
#include "crossover.h"

int main()
{
    srand(time(0));
    readCsv();
    printf("\nread done");
    int remove_count = removeUnfitDataContainers();
    printf("\n%d no. of unfit containers removed, final no. of containers:%d",remove_count,CNT_SIZE);

    createPop();
    geneptr best_genes[NO_CROSSOVER+1];
    elitism(head_population);
    for(int i=0;i<NO_CROSSOVER;i++){
        best_genes[i]=createCopyGene(searchBestGeneEnrg(head_population));
        crossMut(head_population);
        printBestGene(head_population);
    }
    best_genes[NO_CROSSOVER]=searchBestGeneEnrg(head_population);
    printStats(best_genes[NO_CROSSOVER]);
    printBestGeneEnrg(head_population);
    for(int i=0;i<=NO_CROSSOVER;i++){
        printf("\nBest energy consumption of gen %d: %f",i,best_genes[i]->energy_consumption);
    }
    printf("\nend\n");

}
