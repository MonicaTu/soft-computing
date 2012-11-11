#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "GA.h"

#define GAPosRand()            (rand()%GENETIC_LENGTH)
#define BinaryRand()           (rand()%2)
#define SRand()                ((double)rand()/(double)RAND_MAX)

parent_t population[POPULATION_CNT];
parent_t pool[POPULATION_CNT];
parent_t best_gene;

void cal_xvalue(parent_t *x)
{
    int i;
    double dec_x=0, dec_y=0;
    for(i=0; i<GENETIC_LENGTH; i++){
        if(i<(GENETIC_LENGTH/2)) {
            if(x->genes[i] ==1) dec_x = dec_x + (0x01 << ((GENETIC_LENGTH/2-1)-i));
        } else {
            if(x->genes[i] ==1) dec_y = dec_y + (0x01 << ((GENETIC_LENGTH/2-1)-(i-GENETIC_LENGTH/2)));
        }
    }
    x->dec_value_x = (double)dec_x;
    x->dec_value_y = (double)dec_y;

    // adjust value between MAX~MIN
    double c, max=MAX, min=MIN;
    c = (max-min)/(pow(2, GENETIC_LENGTH/2)-1.0);
    x->dec_value_x = (double)(min+dec_x*c);
    x->dec_value_y = (double)(min+dec_y*c);
//    printf("(x:%5.2lf, y:%5.2lf) ", x->dec_value_x, x->dec_value_y);
}

float cal_mean()
{
    int i;
    float sum=0.0, mean=0.0;
    for (i = 0; i < POPULATION_CNT; i++) {
        cal_fitness(&population[i]);
        sum = sum + population[i].fitness;
    }

    mean = sum / (float)POPULATION_CNT;
    return mean;
}

float cal_var()
{
    int i;
    float sum=0.0, var=0.0;
    float mean = cal_mean();
    for (i = 0; i < POPULATION_CNT; i++) {
        sum = sum + pow((population[i].fitness - mean), 2);
    }

    var = sum / (float)POPULATION_CNT;
    return var;
}

void cal_fitness(parent_t *x)
{
    double i = x->dec_value_x;
    double j = x->dec_value_y;
#ifdef SPHERE
    x->fitness = (i*i + j*j);
#endif
#ifdef RASTRIGIN
    x->fitness = (pow(i, 2)-10*cos(2*PI*i)+10) + (pow(j, 2)-10*cos(2*PI*j)+10);
#endif
//        printf("%5.2lf\n", x->fitness);
}

void initialize()
{
    int i, j;
    for(i=0; i<POPULATION_CNT; i++){
//            printf("\n%d: ", i);
        for(j=0; j<GENETIC_LENGTH; j++){
            population[i].genes[j] = BinaryRand();
//            printf("%d", population[i].genes[j]);
        }
        cal_xvalue(&population[i]);
        cal_fitness(&population[i]);

        if(i==0) {
            memcpy(&best_gene,
                    &population[i],
                    sizeof(parent_t));
        } else if (population[i].fitness < best_gene.fitness){
            memcpy(&best_gene,
                    &population[i],
                    sizeof(parent_t));
        }
    }
}

void reproduction()
{
    int i, j, cnt, has_copy=0;
    int Slack = POPULATION_CNT;
    int pos1, pos2;
    double fitness_sum=0.0;

    for(i=0; i<POPULATION_CNT; i++) {
        fitness_sum += population[i].fitness;
    }

    for(i=0; i<POPULATION_CNT && Slack!=0; i++) {
        cnt = (int)(population[i].fitness/fitness_sum + 0.5);
        if(cnt > Slack) cnt=Slack;
        for(j=0; j<cnt; ++j, ++has_copy)
            memcpy(&pool[has_copy],
                    &population[i],
                    sizeof(parent_t));
        Slack-=cnt;
    }
    while(has_copy < POPULATION_CNT){
        pos1 = rand() % POPULATION_CNT;
        do{pos2=rand()%POPULATION_CNT;}while(pos1==pos2);
        if(population[pos1].fitness < population[pos2].fitness) i = pos1;
        else i=pos2;
        memcpy(&pool[has_copy++],&population[i],sizeof(parent_t));
    }
}

void reproduction_rnd()
{
    int i, pos;
    double fitness_sum=0.0;
    double column_prob[POPULATION_CNT];
    double prob;

    for(i=0; i<POPULATION_CNT; i++) {
        fitness_sum += population[i].fitness;
    }
    column_prob[0] = population[0].fitness / fitness_sum;
    for(i=1; i<POPULATION_CNT; ++i){
        column_prob[i] =
            column_prob[i-1] + population[i].fitness / fitness_sum;
    }

    for(i=0; i<POPULATION_CNT; ++i){
        prob=SRand();
        for(pos=0; pos < POPULATION_CNT-1; ++pos){
            if(prob <= column_prob[pos] )
                break;
        }
        memcpy(&pool[i], &population[pos], sizeof(parent_t));
    }
}

void crossover()
{
    int i, cnt=0;
    int pos=0;
    int p1, p2;
    double crossover_if;

    while(cnt < POPULATION_CNT) {
        p1 = rand() % POPULATION_CNT;
        do{p2=rand()% POPULATION_CNT;}while(p2==p1);

        crossover_if = SRand();
        if(crossover_if > CROSSOVER_RATE) {
            memcpy( (void *)&population[cnt++],
                    (void *)&pool[p1],
                    sizeof(parent_t));
            memcpy( (void *)&population[cnt++],
                    (void *)&pool[p2],
                    sizeof(parent_t));
        }
        else {
            do{ pos = GAPosRand();} while(pos==0);
            // crossover
            for(i=0; i<pos; i++){
                population[cnt].genes[i] = pool[p1].genes[i];
                population[cnt+1].genes[i] = pool[p2].genes[i];
            }
            for(i=pos; i<GENETIC_LENGTH; i++) {
                population[cnt+1].genes[i] = pool[p1].genes[i];
                population[cnt].genes[i] = pool[p2].genes[i];
            }
            cnt+=2;
        }
    }
}

void mutation()
{
    int i;
    int pos;
    for(i=0; i<POPULATION_CNT; i++){
        double mutation_if = SRand();
        if(mutation_if <= MUTATION_RATE){
            pos = GAPosRand();
            population[i].genes[pos] =
                1 - population[i].genes[pos];
        }
        cal_xvalue(&population[i]);
        cal_fitness(&population[i]);
        if (population[i].fitness < best_gene.fitness){
            memcpy(&best_gene,
                    &population[i],
                    sizeof(parent_t));
        }
    }
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
