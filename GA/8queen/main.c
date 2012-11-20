#include "GA.h"

int main(int argc, char **argv)
{
    int i;

    srand((unsigned)time(NULL));

    initialize();
/*    print_particles(0);*/

    for(i=1; i<ITERA_CNT+1; i++){
        selection();

        // termination
        if (best == 56) {
          printf("\n(%3d itera)\n", i);
          break;
        }

        if (i == ITERA_CNT)
          printf("\n(%3d itera) not found\n", i);

        crossover();
        mutation();
    }
    printf("\n");
    return 0;
}
