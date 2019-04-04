#include "stdio.h"
#include "stdlib.h"

int 
main(int argc, char const *argv[]){

  double time_user;
  double time_system;
  double time_real;

  char command[100];

  system("dos2unix run_mul.sh");
  printf("#Threads \t real \t user \t sys \n");
  for(int n = 1; n < 13; n++){

    sprintf(command, "sh run_mul.sh %d 2 20 100000000 5 ", n);
    system(command);

    FILE *fp;

    char file_name[50];
    sprintf(file_name, "test_thread%d.txt", n);
    
    fp = fopen(file_name, "r");
    fscanf(fp, "%lfuser %lfsystem 0:%lfelapsed", &time_user, &time_system, &time_real);
    fclose(fp);

    sprintf(file_name, "rm test_thread%d.txt", n);
    system(file_name);

    printf("  %2d \t\t %.2lf \t %.2lf \t %.2lf \n", n, time_real, time_user, time_system);
  }

  return 0;
}
