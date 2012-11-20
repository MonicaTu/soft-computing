#define POPULATION_CNT     10
#define ITERA_CNT          1

#define GENETIC_LENGTH     8
#define CROSSOVER_RATE     0.9
#define MUTATION_RATE      0.02

#define BOARD_LENGTH       8

typedef struct tag_parent_t{
    int genes[GENETIC_LENGTH];
    int fitness;
    int area[8][8];
} parent_t;

void initialize();
void selection();
void crossover();
void mutation();

void cal_fitness(parent_t* t);

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

void print_particles(int num);

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

void cal_fitness(parent_t* t)
{
    int costValue=0;
    int m,n;
    int i,j;
    for(i=0;i<BOARD_LENGTH;i++)
    {
        j=t->genes[i];
//        printf("(%d, %d)\n", i, j);
        // 以該點為中心, 計算其斜率為-1的線,右下方
        m=i+1;
        n=j-1;
        while(m<BOARD_LENGTH && n>=0)
        {
            if(t->area[m][n]==1) {
                costValue++;
//                printf("%dx%d ", m, n);
            }
            m++;
            n--;
        }

        // 以該點為中心, 計算其斜率為+1的線,右上方
        m=i+1;
        n=j+1;
        while(m<BOARD_LENGTH && n<BOARD_LENGTH )
        {
            if(t->area[m][n]==1) {
                costValue++;
//                printf("%dx%d ", m, n);
            }
            m++;
            n++;
        }

        // 以該點為中心, 計算其斜率為+1的線,左下方
        m=i-1;
        n=j-1;
        while(m>=0 && n>=0)
        {
            if(t->area[m][n]==1) {
                costValue++;
//                printf("%dx%d ", m, n);
            }
            m--;
            n--;
        }

        // 以該點為中心, 計算其斜率為-1的線,左上方
        m=i-1;
        n=j+1;
        while(m>=0 && n<BOARD_LENGTH)
        {
            if(t->area[m][n]==1) {
                costValue++;
//                printf("%dx%d ", m, n);
            }
            m--;
            n++;
        }

        // 以該點為中心, 下方
        m=i;
        n=j+1;
        while(n<BOARD_LENGTH)
        {
            if(t->area[m][n]==1) {
                costValue++;
//                printf("%dx%d ", m, n);
            }
            n++;
        }

        // 以該點為中心, 上方
        m=i;
        n=j-1;
        while(n>=0)
        {
            if(t->area[m][n]==1) {
                costValue++;
//                printf("%dx%d ", m, n);
            }
            n--;
        }

        // 以該點為中心, 右方
        m=i+1;
        n=j;
        while(m<BOARD_LENGTH)
        {
            if(t->area[m][n]==1) {
                costValue++;
//                printf("%dx%d ", m, n);
            }
            m++;
        }

        // 以該點為中心, 左方
        m=i-1;
        n=j;
        while(m>=0)
        {
            if(t->area[m][n]==1) {
                costValue++;
//                printf("%dx%d ", m, n);
            }
            m--;
        }
//        printf("(%d, %d) costValue: %d\n", i, j, costValue);
    }

    t->fitness = 28 - costValue;
    printf(" fitness: %d\n", t->fitness);
}

void fill_area(parent_t *t)
{
    int i, j;
    // clear
    for (i=0;i<BOARD_LENGTH;i++)
      for (j=0;j<BOARD_LENGTH;j++)
        t->area[i][j] = 0;

    // fill
    for (i=0;i<BOARD_LENGTH;i++)
      for (j=0;j<BOARD_LENGTH;j++)
        t->area[i][t->genes[i]] = 1;

    // display
//    printf("\n");
//    for (i=0;i<BOARD_LENGTH;i++) {
//      for (j=0;j<BOARD_LENGTH;j++) {
//        printf("%d ", t->area[i][j]);
//      }
//      printf("\n");
//    }

    // debug
//    for (i=0;i<BOARD_LENGTH;i++) {
//      for (j=0;j<BOARD_LENGTH;j++) {
//        if (t->area[i][j] == 1)
//          printf("(%d, %d) ", i, j);
//      }
//    }
//    printf("\n");
}

void initialize()
{
    printf("=========== initialize ===========\n");
    int i, j;
    int p;
    for(i=0; i<POPULATION_CNT; i++){
        printf("%d: ", i);
        for(j=0; j<GENETIC_LENGTH; j++){
            p = (rand() % GENETIC_LENGTH);
            population[i].genes[j] = p;
            printf("%d", population[i].genes[j]);
        }
        fill_area(&population[i]);
        cal_fitness(&population[i]);
    }
}

void selection()
{
    printf("=========== selection ============\n");

    int i, j, cnt, has_copy = 0;
    double fitness_sum = 0.0;
	  double percent = 0.0;
	  double prob_start[BOARD_LENGTH] = {0};
	  double prob_end[BOARD_LENGTH] = {0};
	  double prob = 0.0;

    for(i = 0; i<POPULATION_CNT; i++) {
        cal_fitness(&population[i]);
        fitness_sum += population[i].fitness;
    }

    for(i=0; i<POPULATION_CNT; i++) {
        if (i == 0)
          prob_start[0] = 0.0;
        else
          prob_start[i] = prob_end[i-1];

        percent = (population[i].fitness/fitness_sum);
        prob_end[i] = prob_start[i] + percent;
    }

    for(i=0; i<POPULATION_CNT; i++) {
       prob = SRand();
       printf("rand%d: %f\n", i, prob);

       for(j = 0; j < POPULATION_CNT; ++j, ++has_copy){
          if (prob >= prob_start[j] && prob < prob_start[j])
              memcpy(&pool[has_copy], &population[j], sizeof(parent_t));
       }
    }

    for(i = 0; i < POPULATION_CNT; i++) {
        memcpy(&population[i], &pool[i], sizeof(parent_t));
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
        do {
           p2 = rand()% POPULATION_CNT;
        }while(p2==p1);

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
            do{
                pos = GAPosRand();
            } while(pos==0);
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

//        cal_fitness(&population[p1]);
//        cal_fitness(&population[p2]);
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
//        cal_fitness(&population[i]);

        if (population[i].fitness < best_gene.fitness){
            memcpy(&best_gene,
                    &population[i],
                    sizeof(parent_t));
        }
    }
}

void print_particles(int num)
{
    int i, j;
    parent_t* t;
    for (i=num; i<POPULATION_CNT; i++) {
        t = &population[i];
        printf("\n%d) ", i);
        for (j=0; j<GENETIC_LENGTH; j++) {
            printf("%d", t->genes[j]);
        }
    }
}
