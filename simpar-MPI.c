#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define RND0_1 ((double) random() / ((long long) 1 << 31))
#define G 6.67408e-11
#define EPSLON 0.0005


typedef struct particle {

    double x, y, vx, vy, m;

} particle_t;

void init_particles(long seed, long ncside, long long n_part, particle_t *par, int rank){

    long long i;
    double trash;
    srandom(seed);

    for(i = 0; i < rank * n_part; i++)
        trash = random();

    for(i = 0; i < n_part; i++){

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
main(int argc, char *argv[]){

    if(argc != 5){
        printf("Wrong parameters");
        return 0;
    }

    long seed, ncside,  iterations;
    long long n_part, pos;

    long x, y;
    double dx, dy, aux, d2, ax, ay;

    int id, p;
    long long block_size;

    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    sscanf(argv[1], "%ld",  &seed);
    sscanf(argv[2], "%ld",  &ncside);
    sscanf(argv[4], "%ld",  &iterations);
    sscanf(argv[3], "%lld", &n_part);

    const double epson_sqrt = sqrt(EPSLON);
    const long long cell_size = ncside * ncside;

    //Divide the particles along the processes
    block_size = n_part / p;
    if(!id)
        block_size += n_part % p;

    //printf("Id: %d \t block_size: %lld \t \n", id, block_size);    

    //Creates the particles for each proccess only
    particle_t *par = (particle_t *)malloc(sizeof(particle_t) * block_size);
    init_particles(seed, ncside, block_size, par, id);

    double *cell_x, *cell_y, *cell_m;
    cell_x = (double *)malloc(sizeof(double) * cell_size);
    cell_y = (double *)malloc(sizeof(double) * cell_size);
    cell_m = (double *)malloc(sizeof(double) * cell_size);

    double *cell_x_Global, *cell_y_Global, *cell_m_Global;
    cell_m_Global = (double *)malloc(sizeof(double) * cell_size);
    cell_x_Global = (double *)malloc(sizeof(double) * cell_size);
    cell_y_Global = (double *)malloc(sizeof(double) * cell_size);

    for(int k = 0; k < iterations; k++){

        memset(cell_x, 0, sizeof(double) * cell_size);
        memset(cell_y, 0, sizeof(double) * cell_size);
        memset(cell_m, 0, sizeof(double) * cell_size);

        // Calculate the center of mass of each cell
        // each proccess calculates is particles
        for (long long p = 0; p < block_size ; p++){

        x = (long) (par[p].x * ncside);
        y = (long) (par[p].y * ncside);
        pos = x + ncside * y;

        cell_m[pos] += par[p].m;
        cell_x[pos] += par[p].x * par[p].m;
        cell_y[pos] += par[p].y * par[p].m;
        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(cell_m, cell_m_Global, cell_size, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(cell_x, cell_x_Global, cell_size, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(cell_y, cell_y_Global, cell_size, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        for (long long i = 0; i < cell_size; i++){
            if(cell_m_Global[i] != 0){
                cell_y_Global[i] /= cell_m_Global[i];
                cell_x_Global[i] /= cell_m_Global[i];
            }
        }

        // Compute the gravitational force applied to each particle
        for (long long p = 0; p < block_size; p++){

            x = (long) (par[p].x * ncside) - 2;
            y = (long) (par[p].y * ncside) - 2;

            ax = 0;
            ay = 0;
            for (long i = 0; i < 3; i++){

                x = (x + 1) % ncside;
                if(x == -1)
                x = ncside - 1;

                for (long j = 0; j < 3; j++){

                    y = (y + 1) % ncside;
                    if(y == -1)
                        y = ncside - 1;

                    pos = x + ncside * y;
                    dx = abs(par[p].x - cell_x_Global[pos]);
                    dy = abs(par[p].y - cell_y_Global[pos]);

                    d2 = dx*dx + dy*dy;
                    if(d2 < epson_sqrt)
                        continue;

                    aux = cell_m_Global[pos] / d2 / (dx + dy);
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

    
    double x_total = 0, y_total = 0, m_total = 0;
    for (long long i = 0; i < block_size; i++){

        x = (long) (par[i].x * ncside);
        y = (long) (par[i].y * ncside);

        m_total += par[i].m;
        x_total += par[i].x * par[i].m;
        y_total += par[i].y * par[i].m;
    }

    double x_total_Global = 0, y_total_Global = 0, m_total_Global = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&m_total, &m_total_Global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&x_total, &x_total_Global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&y_total, &y_total_Global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(!id){
        printf("%.2f %.2f \n", par[0].x, par[0].y);
        printf("%.2f %.2f \n", x_total_Global / m_total_Global, y_total_Global / m_total_Global);
    }

    free(par);
    free(cell_x);
    free(cell_y);
    free(cell_m);
    free(cell_m_Global);
    free(cell_y_Global);
    free(cell_x_Global);

    MPI_Finalize();
    return 0;
}

