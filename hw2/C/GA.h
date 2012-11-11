#ifdef SPHERE
#define MAX (100.0)
#define MIN (-100.0)
#define POPULATION_CNT     100
#define ITERA_CNT          100
#endif

#ifdef RASTRIGIN
#define MAX (4.0)
#define MIN (-4.0)
#define PI  (3.141592654)
#define POPULATION_CNT     100
#define ITERA_CNT          100
#endif

#define GENETIC_LENGTH     16
#define CROSSOVER_RATE     0.8
#define MUTATION_RATE      0.1

typedef struct tag_parent_t{
    int genes[GENETIC_LENGTH];
    double fitness;
    double dec_value_x;
    double dec_value_y;
}parent_t;

void initialize();
void reproduction();
void reproduction_rnd();
void crossover();
void mutation();

void cal_fitness(parent_t *x);
void cal_xvalue(parent_t *x);

float cal_mean();
float cal_var();
