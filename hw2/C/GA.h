
#ifdef SPHERE
#define MAX (100.0)
#define MIN (-100.0)
#define POPULATION_CNT   100
#define ITERA_CNT        200
#endif

#ifdef RASTRIGIN
#define MAX (4.0)
#define MIN (-4.0)
#define PI  (3.141592654)
#define POPULATION_CNT     2
#define ITERA_CNT          2
#endif

#define GENETIC_LENGTH     16
#define CROSSOVER_RATE     0.9
#define MUTATION_RATE      0.001

typedef struct tag_parent_t{
    int genes[GENETIC_LENGTH];
    double fitness;
    double dec_value_x;
    double dec_value_y;
}parent_t;

void initialize();
void selection();
void selection_rnd();
void crossover();
void mutation();

void cal_fitness(parent_t *x);
void cal_xvalue(parent_t *x);

float cal_mean();
float cal_var();

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define GAPosRand()            (rand()%GENETIC_LENGTH)
#define BinaryRand()           (rand()%2)
#define SRand()                ((double)rand()/(double)RAND_MAX)
#define IRand()                (int)((double)rand()/(double)RAND_MAX)

parent_t population[POPULATION_CNT];
parent_t pool[POPULATION_CNT];
parent_t best_gene;

void print_particles();
void print_particles();

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
    //printf("(x:%5.2lf, y:%5.2lf) ", x->dec_value_x, x->dec_value_y);
}

float cal_mean()
{
    int i;
    float sum=0.0, mean=0.0;
    for (i = 0; i < POPULATION_CNT; i++) {
/*        printf("+%f\n", population[i].fitness);*/
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
    printf("initialize");
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

void selection()
{
//    printf("\nselection\n");

    int i, j, cnt, has_copy=0;
    int Slack = POPULATION_CNT;
    int pos1, pos2;
    double fitness_sum=0.0;

	  double percent;

    for(i=0; i<POPULATION_CNT; i++) {
        fitness_sum += population[i].fitness;
    }

	  printf("\n");
	  double percent_sum = 0.0;
    for(i=0; i<POPULATION_CNT && Slack!=0; i++) {
/*        cnt = (int)(population[i].fitness/fitness_sum + 0.5);*/
/*        percent = ((((1-(population[i].fitness/fitness_sum))/POPULATION_CNT)-0.04)*100);*/
	  percent = population[i].fitness/fitness_sum;
/*        percent = ((1-percent)/(POPULATION_CNT-1)); */
	  percent = ((1-percent)/(POPULATION_CNT-1));
	  percent = percent*POPULATION_CNT;
/*        percent = percent*POPULATION_CNT;*/
/*        percent = percent*20+0.5;*/
/*        percent = percent+0.5;*/
/*        percent = 1-percent;*/
/*        percent = ((1-(population[i].fitness/fitness_sum))/POPULATION_CNT);*/
/*        percent = percent * POPULATION_CNT;*/
/*        cnt = (int)((1-(population[i].fitness/fitness_sum) + 0.5))*POPULATION_CNT;*/
/*        percent = 1-(population[i].fitness/fitness_sum)+0.5;*/
/*        cnt = (int)(1-(population[i].fitness/fitness_sum)+0.5);*/
/*        cnt = (int)(1-(population[i].fitness/fitness_sum)+0.5);*/
/*        cnt = (int)(percent*POPULATION_CNT+0,5);*/
    cnt = (int)percent;
/*        if (cnt<1)*/
/*                cnt = 0;*/
//        for (j = 0; j<GENETIC_LENGTH; j++)
//          printf("%d", population[i].genes[j]);
//        printf(" %d) percent:%f cnt:%d fitness:%f fitness_sum:%f\n", i, percent, cnt, population[i].fitness, fitness_sum);
/*        if(cnt > Slack) cnt=Slack;*/
/*                  printf("cnt:%d, Slack:%d\n", cnt, Slack);*/
        for(j=0; j<cnt; ++j, ++has_copy){
            memcpy(&pool[has_copy],
                    &population[i],
                    sizeof(parent_t));
//    printf("for) pool[%d], fitness:%f\n", has_copy, pool[has_copy].fitness);
	      }
/*        Slack-=cnt;*/
/*        percent_sum += percent;*/
    }
/*        printf("percent_sum: %f\n", percent_sum);*/
/*    while(has_copy < POPULATION_CNT){*/
/*        pos1 = rand() % POPULATION_CNT;*/
/*        do{pos2=rand()%POPULATION_CNT;}while(pos1==pos2);*/
/*        if(population[pos1].fitness < population[pos2].fitness) i = pos1;*/
/*        else i=pos2;*/
/*        memcpy(&pool[has_copy++],&population[i],sizeof(parent_t));*/
/*    }*/
//printf("start (has_copy: %d)\n", has_copy);
    while(has_copy < POPULATION_CNT){
          if (has_copy < 2) {
            printf("has_copy = 1, pos1 = 1, pos2 = 1\n");
            pos1 = 1;
            pos2 = 1;
          } else {
              pos1 = rand() % has_copy;

              do{
                  pos2 = rand() % has_copy;
              } while(pos1 == pos2);
        }

        if(pool[pos1].fitness < pool[pos2].fitness)
          i = pos1;
        else
          i=pos2;
        memcpy(&pool[has_copy++],&pool[i],sizeof(parent_t));
//    printf("while) pool[%d], fitness:%f\n", has_copy, pool[has_copy-1].fitness);
    }
//printf("end\n");
	for(i = 0; i < POPULATION_CNT; i++) {
		memcpy(&population[i], &pool[i], sizeof(parent_t));
	}
//printf("end\n");
}

void selection_rnd()
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
        p1 =  rand() % POPULATION_CNT;
        do{p2=  rand()% POPULATION_CNT;}while(p2==p1);

//  printf("\np1:%d, p2:%d", p1, p2);

        crossover_if = SRand();
        if(crossover_if > CROSSOVER_RATE) {
//            printf("\n!crossover");
            memcpy( (void *)&population[p1],
                    (void *)&pool[p1],
                    sizeof(parent_t));
            memcpy( (void *)&population[p2],
                    (void *)&pool[p2],
                    sizeof(parent_t));
        }
        else {
//            do{
                pos = GAPosRand();
//            } while(pos==0);
//            printf(" pos:%d \n", pos);
            // crossover
            for(i=0; i<pos; i++){
		            population[p1].genes[i] = pool[p1].genes[i];
		            population[p2].genes[i] = pool[p2].genes[i];
            }
            for(i=pos; i<GENETIC_LENGTH; i++) {
                population[p2].genes[i] = pool[p1].genes[i];
                population[p1].genes[i] = pool[p2].genes[i];
            }
            cnt+=2;
        }

        cal_xvalue(&population[p1]);
        cal_fitness(&population[p1]);
        cal_xvalue(&population[p2]);
        cal_fitness(&population[p2]);
    }
}

void mutation()
{
    int i;
    int pos;

    for(i=0; i<POPULATION_CNT; i++){
        double mutation_if = SRand();
        if(mutation_if <= MUTATION_RATE){
            printf("\nmutation");

            pos = GAPosRand();
//            printf("mutation_if:%f\n", mutation_if);
//            printf("pos:%d\n", pos);
//            getchar();
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

//void print_particles()
//{
//  int i;
//  parent_t* t;
//  printf("\n======================================\n");
//  for (i=0; i<POPULATION_CNT; i++) {
//    t = &population[i];
//    printf("(%5.2lf, %5.2lf) %5.2lf\n",
//      t->dec_value_x,
//      t->dec_value_y,
//      t->fitness);
//  }
//  printf("======================================\n");
//}

void print_particles()
{
	int i, j;
  parent_t* t;
	printf("\n======================================\n");
	for (i=0; i<POPULATION_CNT; i++) {
      t = &population[i];
	    printf("%d) ", i);
	    for (j=0; j<GENETIC_LENGTH; j++) {
		    printf("%d", t->genes[j]);
	    }
      printf(" (%5.2lf, %5.2lf) %5.2lf\n",
        t->dec_value_x,
        t->dec_value_y,
        t->fitness);
	}
	printf("\n======================================\n");
}

void print_ever_best()
{
	printf(" || ever best: (%5.2lf, %5.2lf) %5.2lf\n",
		best_gene.dec_value_x,
		best_gene.dec_value_y,
		best_gene.fitness);
}
