#include <iostream>
#include <limits>

#define RND0_1 ((double) random() / ((long long) 1 << 31))
#define G 6.67408e-11
#define EPSLON 0.01

typedef struct particle {

  double x, y, vx, vy, m, cell;

} particle_t;

void init_particles(long seed, long ncside, long long n_part, particle_t *par)
{
    long long i;
    srandom(seed);

    for(i = 0; i < n_part; i++)
    {
        par[i].x = RND0_1;
        par[i].y = RND0_1;
        par[i].vx = RND0_1 / ncside / 10.0;
        par[i].vy = RND0_1 / ncside / 10.0;

        par[i].m = RND0_1 * ncside / (G * 1e6 * n_part);
    }
}

int 
main(){

  long seed = 1;
  long ncside = 15;
  long long n_part = 200000;

  double **cell;
  cell = (double **) malloc (sizeof(double *)  * ncside);
  for(int i = 0; i < ncside; i++)
    cell[i] = (double *) malloc (sizeof(double) * ncside);

  for (int i = 0; i < ncside; i++){
    for (int j = 0; j < ncside; j++){
      cell[i][j] = 0;
    }
  }

  particle_t *par = (particle_t *)malloc(sizeof(particle_t) * n_part);
  init_particles(seed, ncside, n_part, par);

  double mass_total = 0;

  for(long long i = 0; i < n_part; i++){

    
  }

  std::cout << "Total mass " << " = " << mass_total << std::endl;


  free(par);
  for (int i = 0; i < ncside; i++){
    free(cell[i]);
  }
  free(cell);
}