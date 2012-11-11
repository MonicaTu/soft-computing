#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "GA.h"

void print_ever_best()
{
    printf("ever find best gene : ");
    printf("(%5.2lf, %5.2lf) %5.2lf\n", best_gene.dec_value_x, best_gene.dec_value_y, best_gene.fitness);
}

void print_particles(int i)
{
    int j;

    printf("=========================\n");
    for(j=0; j<POPULATION_CNT; j++){
        printf("(%5.2lf, %5.2lf) %5.2lf\n", population[j].dec_value_x, population[j].dec_value_y, population[j].fitness);
    }
    printf("=========================\n");
}

int main(int argc, char **argv)
{
    int i;

    srand((unsigned)time(NULL));
    initialize();
    for(i=0; i<ITERA_CNT; i++){
        reproduction();
        crossover();
        mutation();

        printf("\n(%3d itera) ", i);

        float mean = cal_mean();
        printf("mean: %12.2lf; ", mean);

        float var = cal_var();
        printf("var: %12.2lf; ", var);

/*        print_particles(i);*/
/*        print_ever_best();*/
    }
    printf("\n");
    return 0;
}


