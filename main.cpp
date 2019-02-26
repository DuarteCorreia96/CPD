#include <iostream>

#define RND0_1 ((double) random() / ((long long) 1 <<31))
#define G 6.67408e-11
#define EPSLON 0.01

typedef struct particle {

  double x, y, vx, vy, m;

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
  long ncside = 3;
  long long n_part = 30;

  particle_t *par = (particle_t *) malloc(sizeof(particle_t) * n_part);

  init_particles(seed, ncside, n_part, par);

  
  for(int i = 0; i < n_part; i++){

    std::cout << "Par " << i << " = " << par[i].m << std::endl;
  }
  
  free(par);

}