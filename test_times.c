#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int 
main(int argc, char const *argv[]){

  long seed = 3;
  long ncside = 5;
  long iterations = 5;
  long long n_part = 500;

  double n_iter = 100;
  double time_user;
  double time_system;
  double time_real;
  double cpu_usage;

  FILE *fp, *results, *time_precise;
  char line[1028];
  char command[100];
  char file_name[50];

  sprintf(file_name, "results_%ld_%ld_%lld_%ld.txt", seed, ncside, n_part, iterations);
  results = fopen(file_name, "w");

  system("dos2unix run_mul.sh");

  fprintf(results, " Number of runs averaged: %.0lf \n\n", n_iter);
  fprintf(results, "%%{\n Seed: %ld\n Sides: %ld\n Iterations: %ld\n Particles: %lld\n%%}\n\n", seed, ncside, iterations, n_part);
  fprintf(results, "#Threads     real         user       sys      CPU %%\n");
  printf("#Threads    real         user       sys      CPU %%\n");
  for(int n = 1; n < 13; n++){

    sprintf(command, "sh run_mul.sh %d %ld %ld %lld %ld ", n, seed, ncside, n_part, iterations);
    double time_user_acc = 0;
    double time_system_acc = 0;
    double time_real_acc = 0;
    double cpu_usage_acc = 0;

    for(int i = 0; i < (int) n_iter; i++){

      system(command);
      sprintf(file_name, "test_thread%d.txt", n);
      
      fp = fopen(file_name, "r");
      fgets(line, sizeof(line), fp);
      fclose(fp);

      sscanf(line, "%lfuser %lfsystem 0:%lflapsed %lf ", &time_user, &time_system, &time_real, &cpu_usage);

      time_precise = fopen("thrash.txt", "r");
      fgets(line, sizeof(line), time_precise);

      sscanf(line, "%lg\n", &time_real);
      fclose(time_precise);

      sprintf(file_name, "rm test_thread%d.txt", n);
      system(file_name);

      time_user_acc   += time_user;
      time_system_acc += time_system;
      time_real_acc   += time_real;
      cpu_usage_acc   += cpu_usage;
    }

    fprintf(results, "[  %2d       %8.5lf   %8.3lf   %8.3lf   %4.0lf]; ...\n", n, time_real_acc / n_iter, time_user_acc / n_iter, time_system_acc / n_iter, cpu_usage_acc / n_iter);
    printf("  %2d       %8.5lf   %8.3lf   %8.3lf   %4.0lf \n", n, time_real_acc / n_iter, time_user_acc / n_iter, time_system_acc / n_iter, cpu_usage_acc / n_iter);
  }

  fclose(results);
  return 0;
}
