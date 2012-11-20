#define POPULATION_CNT     100
#define ITERA_CNT          5000

#define GENETIC_LENGTH     8
#define CROSSOVER_RATE     0.8
#define MUTATION_RATE      0.01

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
parent_t *solution;
int best;

void print_particles(int num);
void print_area(parent_t* t);

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
#if 1
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
#endif
//        printf("(%d, %d) costValue: %d\n", i, j, costValue);
    }

    if (costValue == 0) {
        solution = t;
    }

    t->fitness = 56 - costValue;

    if (t->fitness > best) {
      best = t->fitness;
//      printf("best: %d\n", best);
    }
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

#if 0 // debug
    print_area(t);

    for (i=0;i<BOARD_LENGTH;i++) {
      for (j=0;j<BOARD_LENGTH;j++) {
        if (t->area[i][j] == 1)
          printf("(%d, %d) ", i, j);
      }
    }
    printf("\n");
#endif
}

void initialize()
{
//    printf("=========== initialize ===========\n");
    int i, j;
    int p;
    for(i=0; i<POPULATION_CNT; i++){
//        printf("%d: ", i);
        for(j=0; j<GENETIC_LENGTH; j++){
            p = (rand() % GENETIC_LENGTH);
            population[i].genes[j] = p;
//            printf("%d", population[i].genes[j]);
        }
        fill_area(&population[i]);
        cal_fitness(&population[i]);
//        printf("\n");
    }
}

void selection()
{
//    printf("\n=========== selection ============\n");

    int i, j, cnt, has_copy = 0;
    double fitness_sum = 0.0;
	  double percent = 0.0;
	  double prob_start[POPULATION_CNT] = {0};
	  double prob_end[POPULATION_CNT] = {0};
	  double prob = 0.0;

    for(i = 0; i<POPULATION_CNT; i++) {
        fitness_sum += population[i].fitness;
    }
#if 1
    for(i=0; i<POPULATION_CNT; i++) {
        if (i == 0)
          prob_start[0] = 0.0;
        else
          prob_start[i] = prob_end[i-1];

        percent = (population[i].fitness/fitness_sum);
        prob_end[i] = prob_start[i] + percent;
//        printf("%d) %f[%f~%f]\n", i, percent, prob_start[i], prob_end[i]);
    }

    for(i=0; i<POPULATION_CNT; i++) {
       prob = SRand();
//       printf("prob: %f ", prob);
       for(j = 0; j < POPULATION_CNT; j++){
          if (prob >= prob_start[j] && prob < prob_end[j]) {
//              printf("j: %d\n", j);
              memcpy(&pool[has_copy], &population[j], sizeof(parent_t));
              has_copy++;
              break;
          }
       }
    }
#else
    int choose;
    for(i=0; i<POPULATION_CNT; i++) {
        percent = (population[i].fitness/fitness_sum);
//            printf("percent: %f\n", percent);
        if (percent >= 1.0/POPULATION_CNT) {
            memcpy(&pool[has_copy], &population[i], sizeof(parent_t));
            has_copy++;
        }
    }

//    printf("has_copy: %d\n", has_copy);
    for(i=has_copy; i<POPULATION_CNT; i++) {
       if (i == 0)
          break;
       choose = rand() % has_copy;
//       printf("choose: %d\n", choose);
       memcpy(&pool[has_copy], &pool[choose], sizeof(parent_t));
       has_copy++;
    }
#endif

    for(i = 0; i < POPULATION_CNT; i++) {
        memcpy(&population[i], &pool[i], sizeof(parent_t));
        // update
        fill_area(&population[i]);
        cal_fitness(&population[i]);
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
        } while(p2==p1);
//        printf("\np1:%d, p2:%d", p1, p2);

        crossover_if = SRand();
        if(crossover_if > CROSSOVER_RATE) {
            memcpy( (void *)&population[p1],
                    (void *)&pool[p1],
                    sizeof(parent_t));
            memcpy( (void *)&population[p2],
                    (void *)&pool[p2],
                    sizeof(parent_t));
        }
        else {
//            printf("\n=========== crossover ============\n");
            do{
                pos = GAPosRand();
//                printf("pos:%d\n", pos);
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
        }
        fill_area(&population[p1]);
        cal_fitness(&population[p1]);
        fill_area(&population[p2]);
        cal_fitness(&population[p2]);
        cnt+=2;
//            printf("cnt: %d\n", cnt);
    }
}

void mutation()
{
    int i, j;
    int pos;
    int mutation_value;
    double mutation_if;

    for(i = 0; i<POPULATION_CNT; i++){
      for (j = 0; j < GENETIC_LENGTH; j++) {
        mutation_if = SRand();
        mutation_value =  rand() % GENETIC_LENGTH;
//        printf("mutation_if: %f\n", mutation_if);
        if(mutation_if <= MUTATION_RATE){
//          printf("\n=========== mutation ============\n");
//          printf("mutation_value: %d\n", mutation_value);
          population[i].genes[j] = mutation_value;
        }
      }
      fill_area(&population[i]);
      cal_fitness(&population[i]);
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
        printf(" fitness: %d", t->fitness);
    }
}

void print_area(parent_t* t) {
    int i, j;
    printf("\n");
    for (i=0;i<BOARD_LENGTH;i++) {
        for (j=0;j<BOARD_LENGTH;j++) {
            printf("%d ", t->area[i][j]);
        }
        printf("\n");
    }
}
