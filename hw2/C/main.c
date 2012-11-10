/*******************************************************************/
/*                                                                 */
/*     filename : GaMain.c                                         */
/*     author   : edison.shih/edisonx                              */
/*     compiler : Visual C++ 2008                                  */
/*     date     : 2010.02.14                                       */
/*     maintain : 2011.11.21                                       */
/*     maintain : 2012.06.15                                       */
/*                                                                 */
/*         A.L.L.      R.I.G.H.T.S.     R.E.S.E.R.V.E.             */
/*                                                                 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "GA.h"

void print_result(int i)
{
    int j;

    printf("\n =========================\n");
    printf(" %3d times..\n", i);
    for(j=0; j<POPULATION_CNT; j++){
        printf("(%5.2lf, %5.2lf, %5.2lf) ", population[j].dec_value_x, population[j].dec_value_y, population[j].fitness);
        if(j%4==3) printf("\n");
    }
    printf("\n =========================\n");
    printf(" ever find best gene : ");
    printf("(%5.2lf, %5.2lf, %5.2lf)\n", best_gene.dec_value_x, best_gene.dec_value_y, best_gene.fitness);
}

int main(int argc, char **argv)
{
    int i;

    srand((unsigned)time(NULL));
    initialize();            // 初始化
    for(i=0; i<ITERA_CNT; i++){
        reproduction();      // 選擇(分配式)
        //  reproduction_rnd();  // 選擇(隨機式), 收斂速度慢
        crossover();         // 交配
        mutation();          // 突變f

        print_result(i);
    }
    return 0;
}


