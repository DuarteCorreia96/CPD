#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define RND0_1 ((double) random() / ((long long) 1 << 31))
#define G 6.67408e-11
#define EPSLON 0.0005

typedef struct particle {

  double x, y, vx, vy, m;

} particle_t;

void init_particles(long seed, long ncside, long long n_part, particle_t *par){
  
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

double
check_limits(double position){

  if(position < 0) 
    position += 1;

  else if(position > 1)
    position -= 1;

  return position;
}

int 
main(int argc, char const *argv[]){

  if(argc != 5){
    printf("Wrong parameters");
    return 0;
  }

  long x, y;
  long seed, ncside, iterations;
  long long n_part, i, j, k, p, pos, counter;
  double dx, dy, aux, d2, ax, ay;

  sscanf(argv[1], "%ld", &seed);
  sscanf(argv[2], "%ld", &ncside);
  sscanf(argv[4], "%ld", &iterations);
  sscanf(argv[3], "%lld", &n_part);

  const long long cell_size = ncside * ncside;
  const double epson_sqrt = sqrt(EPSLON);

  particle_t *par = (particle_t *)malloc(sizeof(particle_t) * n_part);
  init_particles(seed, ncside, n_part, par);

  double *cell_x, *cell_y, *cell_m;
  cell_x = (double *)malloc(sizeof(double) * cell_size);
  cell_y = (double *)malloc(sizeof(double) * cell_size);
  cell_m = (double *)malloc(sizeof(double) * cell_size);

  for(k = 0; k < iterations; k++){

    memset(cell_x, 0, sizeof(double)* cell_size);
    memset(cell_y, 0, sizeof(double) * cell_size);
    memset(cell_m, 0, sizeof(double) * cell_size);

    // Calculate the center of mass of each cell
    #pragma omp parallel for private(x, y, pos) \
    reduction(+:cell_x[:cell_size], cell_y[:cell_size], cell_m[:cell_size])
    for (p= 0; p < n_part; p++){

      x = (long)(par[p].x * ncside);
      y = (long)(par[p].y * ncside);
      pos = x + ncside * y;

      cell_m[pos] += par[p].m;
      cell_x[pos] += par[p].x * par[p].m;
      cell_y[pos] += par[p].y * par[p].m;
    }

    #pragma omp parallel for
    for (i = 0; i < cell_size; i++){
      if(cell_m[i] != 0){
        cell_y[i] /= cell_m[i];
        cell_x[i] /= cell_m[i];
      }
    }

    // Compute the gravitational force applied to each particle
    #pragma omp parallel for private(i, j, p, pos, aux, d2, x, y, ax, ay , dx, dy)
    for (p = 0; p < n_part; p ++){

      x = (long) (par[p].x * ncside) - 2;
      y = (long) (par[p].y * ncside) - 2;

      ax = 0;
      ay = 0;
      for (i = 0; i < 3; i++){

        x = (x + 1) % ncside;
        if(x == -1)  
          x = ncside - 1; 

        for (j = 0; j < 3; j++){

          y = (y + 1) % ncside;
          if(y == -1)  
            y = ncside - 1;

          pos = x + ncside * y;
          dx = abs(par[p].x - cell_x[pos]);
          dy = abs(par[p].y - cell_y[pos]);

          d2 = dx*dx + dy*dy;
          if(d2 < epson_sqrt) 
            continue;

          aux = cell_m[pos] / d2 / (dx + dy);
          ax += aux * dx;
          ay += aux * dy;
        }
      }

      ax *= G;
      ay *= G;

      // Calculate the new velocity and then the new position of each particle
      par[p].x += par[p].vx + ax / 2;
      par[p].y += par[p].vy + ay / 2;

      par[p].x = check_limits(par[p].x);
      par[p].y = check_limits(par[p].y);

      par[p].vx += ax;
      par[p].vy += ay;
    }
  
  }

  printf("%.2f %.2f \n", par[0].x, par[0].y);
  double x_total = 0, y_total = 0, m_total = 0;
  #pragma omp parallel for reduction(+:m_total, x_total, y_total)
  for (i = 0; i < n_part; i++){

    m_total += par[i].m;
    x_total += par[i].x * par[i].m; 
    y_total += par[i].y * par[i].m;  
  }

  printf("%.2f %.2f \n", x_total / m_total, y_total / m_total);

  free(par);
  free(cell_x);
  free(cell_y);
  free(cell_m);
  return 0;
}