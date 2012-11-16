#include "GA.h"

int main(int argc, char **argv)
{
    int i;

    srand((unsigned)time(NULL));

/*for (i = 0; i< 10; i++)*/
/*          printf("rand: %d\n", rand());*/
#if 1
    initialize();
/*    print_particles();*/
    for(i=0; i<ITERA_CNT; i++){
/*        print_particles();*/
        selection();
/*        print_particles();*/

/*  print_particles();*/
        crossover();
/*  print_particles();*/

/*        print_particles();*/

        mutation();
/*        print_particles();*/

        printf("\n(%3d itera) ", i);

/*        print_particles();*/
        float mean = cal_mean();
        printf(" || mean: %12.10lf; ", mean);

        float var = cal_var();
        printf(" || var: %12.10lf; ", var);

/*        print_particles();*/
/*        print_particles();*/
/*        print_ever_best();*/
/*getchar();*/
    }
    printf("\n");
#endif
    return 0;
}
