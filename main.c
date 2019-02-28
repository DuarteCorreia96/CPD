#include <stdio.h>
#include <stdlib.h>

#define RND0_1 ((double) random() / ((long long) 1 << 31))
#define G 6.67408e-11
#define EPSLON 0.01

typedef struct particle {

  double x, y, vx, vy, ax, ay, m;

} particle_t;

typedef struct cell_blocks{

  double x, y, m;

} cell_t;

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
  
  long seed, ncside,  iterations;
  long long n_part;
  
  long x, y;
  double dx, dy, aux, d2;

  sscanf(argv[1], "%ld", &seed);
  sscanf(argv[2], "%ld", &ncside);
  sscanf(argv[4], "%ld", &iterations);
  sscanf(argv[3], "%lld", &n_part);

  particle_t *par = (particle_t *)malloc(sizeof(particle_t) * n_part);
  init_particles(seed, ncside, n_part, par);

  cell_t **cell;
  cell = (cell_t **) malloc (sizeof(cell_t *)  * ncside);
  for(long i = 0; i < ncside; i++)
    cell[i] = (cell_t *) malloc (sizeof(cell_t) * ncside);

  for(int k = 0; k < iterations; k++){
    
    for (long i = 0; i < ncside; i++){
      for (long j = 0; j < ncside; j++){
        cell[i][j].x = 0;
        cell[i][j].y = 0;
        cell[i][j].m = 0;
      }
    }
  
    // Calculate the center of mass of each cell
    for (long long i = 0; i < n_part; i++){
      
      x = (long) (par[i].x * ncside);
      y = (long) (par[i].y * ncside);
      
      cell[x][y].m += par[i].m;
      cell[x][y].x += par[i].x * par[i].m; 
      cell[x][y].y += par[i].y * par[i].m;  
    }

    for (long i = 0; i < ncside; i++){
      for (long j = 0; j < ncside; j++){
        if(cell[i][j].m != 0){
          cell[i][j].x /= cell[i][j].m;
          cell[i][j].y /= cell[i][j].m;
        }
      }
    }

    // Compute the gravitational force applied to each particle
    for (long long p = 0; p < n_part; p++){

      x = (long) (par[p].x * ncside) - 2;
      y = (long) (par[p].y * ncside) - 2;

      par[p].ax = 0;
      par[p].ay = 0;
      for (long i = 0; i < 3; i++){

        x = (x + 1) % ncside;
        if(x == -1)  
          x = ncside - 1; 

        for (long j = 0; j < 3; j++){

          y = (y + 1) % ncside;
          if(y == -1)  
            y = ncside - 1;

          dx = abs(par[p].x - cell[x][y].x);
          dy = abs(par[p].y - cell[x][y].y);

          d2 = dx*dx + dy*dy;
          if(d2 < 0.1) 
            continue;

          aux = cell[x][y].m / d2 / (dx + dy);
          par[p].ax += aux * dx;
          par[p].ay += aux * dy;
        }
      }

      par[p].ax *= G;
      par[p].ay *= G;
    }

    // Calculate the new velocity and then the new position of each particle
    for (long long p = 0; p < n_part; p++){

      par[p].x += par[p].vx + par[p].ax / 2;
      par[p].y += par[p].vy + par[p].ay / 2; 

      par[p].x = check_limits(par[p].x);
      par[p].y = check_limits(par[p].y);

      par[p].vx += par[p].ax;
      par[p].vy += par[p].ay; 
    }
  }

  printf("%.2f %.2f \n", par[0].x, par[0].y);
  double x_total = 0, y_total = 0, m_total = 0;
  for (long long i = 0; i < n_part; i++){

    x = (long) (par[i].x * ncside);
    y = (long) (par[i].y * ncside);

    m_total += par[i].m;
    x_total += par[i].x * par[i].m; 
    y_total += par[i].y * par[i].m;  
  }

  printf("%.2f %.2f \n", x_total / m_total, y_total / m_total);

  free(par);
  for (long i = 0; i < ncside; i++)
    free(cell[i]);
  free(cell);
  return 0;
}