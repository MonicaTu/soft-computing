#include "GA.h"

int main(int argc, char **argv)
{
    int i;

    srand((unsigned)time(NULL));

    initialize();
    print_particles(0);

    for(i=0; i<ITERA_CNT; i++){
        selection();

        crossover();
        mutation();

        printf("\n(%3d itera) ", i);

        float mean = cal_mean();
        printf(" || mean: %10.10lf;\t", mean);

        float var = cal_var();
        printf(" || var: %10.10lf;", var);
        if ( var < 0.0000000001 ) {
          print_particles((POPULATION_CNT-1));
          break;
        }
    }
    printf("\n");
    return 0;
}
